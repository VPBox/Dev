/******************************************************************************
 *
 *  Copyright 2014 Google, Inc.
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at:
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 ******************************************************************************/

#include <base/logging.h>
#include <string.h>

#include <mutex>

#include "osi/include/allocator.h"
#include "osi/include/fixed_queue.h"
#include "osi/include/list.h"
#include "osi/include/osi.h"
#include "osi/include/reactor.h"
#include "osi/include/semaphore.h"

typedef struct fixed_queue_t {
  list_t* list;
  semaphore_t* enqueue_sem;
  semaphore_t* dequeue_sem;
  std::mutex* mutex;
  size_t capacity;

  reactor_object_t* dequeue_object;
  fixed_queue_cb dequeue_ready;
  void* dequeue_context;
} fixed_queue_t;

static void internal_dequeue_ready(void* context);

fixed_queue_t* fixed_queue_new(size_t capacity) {
  fixed_queue_t* ret =
      static_cast<fixed_queue_t*>(osi_calloc(sizeof(fixed_queue_t)));

  ret->mutex = new std::mutex;
  ret->capacity = capacity;

  ret->list = list_new(NULL);
  if (!ret->list) goto error;

  ret->enqueue_sem = semaphore_new(capacity);
  if (!ret->enqueue_sem) goto error;

  ret->dequeue_sem = semaphore_new(0);
  if (!ret->dequeue_sem) goto error;

  return ret;

error:
  fixed_queue_free(ret, NULL);
  return NULL;
}

void fixed_queue_free(fixed_queue_t* queue, fixed_queue_free_cb free_cb) {
  if (!queue) return;

  fixed_queue_unregister_dequeue(queue);

  if (free_cb)
    for (const list_node_t* node = list_begin(queue->list);
         node != list_end(queue->list); node = list_next(node))
      free_cb(list_node(node));

  list_free(queue->list);
  semaphore_free(queue->enqueue_sem);
  semaphore_free(queue->dequeue_sem);
  delete queue->mutex;
  osi_free(queue);
}

void fixed_queue_flush(fixed_queue_t* queue, fixed_queue_free_cb free_cb) {
  if (!queue) return;

  while (!fixed_queue_is_empty(queue)) {
    void* data = fixed_queue_try_dequeue(queue);
    if (free_cb != NULL) {
      free_cb(data);
    }
  }
}

bool fixed_queue_is_empty(fixed_queue_t* queue) {
  if (queue == NULL) return true;

  std::lock_guard<std::mutex> lock(*queue->mutex);
  return list_is_empty(queue->list);
}

size_t fixed_queue_length(fixed_queue_t* queue) {
  if (queue == NULL) return 0;

  std::lock_guard<std::mutex> lock(*queue->mutex);
  return list_length(queue->list);
}

size_t fixed_queue_capacity(fixed_queue_t* queue) {
  CHECK(queue != NULL);

  return queue->capacity;
}

void fixed_queue_enqueue(fixed_queue_t* queue, void* data) {
  CHECK(queue != NULL);
  CHECK(data != NULL);

  semaphore_wait(queue->enqueue_sem);

  {
    std::lock_guard<std::mutex> lock(*queue->mutex);
    list_append(queue->list, data);
  }

  semaphore_post(queue->dequeue_sem);
}

void* fixed_queue_dequeue(fixed_queue_t* queue) {
  CHECK(queue != NULL);

  semaphore_wait(queue->dequeue_sem);

  void* ret = NULL;
  {
    std::lock_guard<std::mutex> lock(*queue->mutex);
    ret = list_front(queue->list);
    list_remove(queue->list, ret);
  }

  semaphore_post(queue->enqueue_sem);

  return ret;
}

bool fixed_queue_try_enqueue(fixed_queue_t* queue, void* data) {
  CHECK(queue != NULL);
  CHECK(data != NULL);

  if (!semaphore_try_wait(queue->enqueue_sem)) return false;

  {
    std::lock_guard<std::mutex> lock(*queue->mutex);
    list_append(queue->list, data);
  }

  semaphore_post(queue->dequeue_sem);
  return true;
}

void* fixed_queue_try_dequeue(fixed_queue_t* queue) {
  if (queue == NULL) return NULL;

  if (!semaphore_try_wait(queue->dequeue_sem)) return NULL;

  void* ret = NULL;
  {
    std::lock_guard<std::mutex> lock(*queue->mutex);
    ret = list_front(queue->list);
    list_remove(queue->list, ret);
  }

  semaphore_post(queue->enqueue_sem);

  return ret;
}

void* fixed_queue_try_peek_first(fixed_queue_t* queue) {
  if (queue == NULL) return NULL;

  std::lock_guard<std::mutex> lock(*queue->mutex);
  return list_is_empty(queue->list) ? NULL : list_front(queue->list);
}

void* fixed_queue_try_peek_last(fixed_queue_t* queue) {
  if (queue == NULL) return NULL;

  std::lock_guard<std::mutex> lock(*queue->mutex);
  return list_is_empty(queue->list) ? NULL : list_back(queue->list);
}

void* fixed_queue_try_remove_from_queue(fixed_queue_t* queue, void* data) {
  if (queue == NULL) return NULL;

  bool removed = false;
  {
    std::lock_guard<std::mutex> lock(*queue->mutex);
    if (list_contains(queue->list, data) &&
        semaphore_try_wait(queue->dequeue_sem)) {
      removed = list_remove(queue->list, data);
      CHECK(removed);
    }
  }

  if (removed) {
    semaphore_post(queue->enqueue_sem);
    return data;
  }
  return NULL;
}

list_t* fixed_queue_get_list(fixed_queue_t* queue) {
  CHECK(queue != NULL);

  // NOTE: Using the list in this way is not thread-safe.
  // Using this list in any context where threads can call other functions
  // to the queue can break our assumptions and the queue in general.
  return queue->list;
}

int fixed_queue_get_dequeue_fd(const fixed_queue_t* queue) {
  CHECK(queue != NULL);
  return semaphore_get_fd(queue->dequeue_sem);
}

int fixed_queue_get_enqueue_fd(const fixed_queue_t* queue) {
  CHECK(queue != NULL);
  return semaphore_get_fd(queue->enqueue_sem);
}

void fixed_queue_register_dequeue(fixed_queue_t* queue, reactor_t* reactor,
                                  fixed_queue_cb ready_cb, void* context) {
  CHECK(queue != NULL);
  CHECK(reactor != NULL);
  CHECK(ready_cb != NULL);

  // Make sure we're not already registered
  fixed_queue_unregister_dequeue(queue);

  queue->dequeue_ready = ready_cb;
  queue->dequeue_context = context;
  queue->dequeue_object =
      reactor_register(reactor, fixed_queue_get_dequeue_fd(queue), queue,
                       internal_dequeue_ready, NULL);
}

void fixed_queue_unregister_dequeue(fixed_queue_t* queue) {
  CHECK(queue != NULL);

  if (queue->dequeue_object) {
    reactor_unregister(queue->dequeue_object);
    queue->dequeue_object = NULL;
  }
}

static void internal_dequeue_ready(void* context) {
  CHECK(context != NULL);

  fixed_queue_t* queue = static_cast<fixed_queue_t*>(context);
  queue->dequeue_ready(queue, queue->dequeue_context);
}
