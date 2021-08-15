#pragma once
#include <memory>

struct alarm_t;

class AlarmMock {
 public:
  MOCK_METHOD1(AlarmNew, alarm_t*(const char*));
  MOCK_METHOD1(AlarmFree, void(alarm_t*));
  MOCK_METHOD1(AlarmCancel, void(alarm_t*));
  MOCK_METHOD4(AlarmSetOnMloop, void(alarm_t* alarm, uint64_t interval_ms,
                                     alarm_callback_t cb, void* data));

  alarm_t* AlarmNewImpl(const char* name) {
    AlarmNew(name);
    // We must return something from alarm_new in tests, if we just return null,
    // unique_ptr will misbehave. Just reserve few bits they will be freed in
    // AlarmFreeImpl
    return (alarm_t*)new uint8_t[30];
  }

  void AlarmFreeImpl(alarm_t* alarm) {
    uint8_t* ptr = (uint8_t*)alarm;
    delete[] ptr;
    return AlarmFree(alarm);
  }

  static inline AlarmMock* Get() {
    if (!localAlarmMock) {
      localAlarmMock = std::make_unique<AlarmMock>();
    }
    return localAlarmMock.get();
  }

  static inline void Reset() { localAlarmMock = std::make_unique<AlarmMock>(); }

 private:
  static std::unique_ptr<AlarmMock> localAlarmMock;
};

std::unique_ptr<AlarmMock> AlarmMock::localAlarmMock;

alarm_t* alarm_new(const char* name) {
  return AlarmMock::Get()->AlarmNewImpl(name);
}

void alarm_free(alarm_t* alarm) { AlarmMock::Get()->AlarmFreeImpl(alarm); }

void alarm_set_on_mloop(alarm_t* alarm, uint64_t interval_ms,
                        alarm_callback_t cb, void* data) {
  AlarmMock::Get()->AlarmSetOnMloop(alarm, interval_ms, cb, data);
}
