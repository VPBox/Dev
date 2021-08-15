# Copyright (C) 2016 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

from __future__ import print_function, absolute_import

import functools
import warnings
import inspect


class skip_conditional(object):
    '''
    Test method decorator that marks a test method as ignorable if the given
    arguments evaluate as Truthy. If the argument is callable, then it is called
    and the return value is used as the predicate.

    >>> class MyTestClass(TestBase):
    ...     def test_something(self):
    ...         pass
    ...
    ...     @skip_conditional(not sys.platform.startswith("linux"))
    ...     def test_some_linux_behaviour(self):
    ...         assert "vmlinuz" in open("/proc/cmdline").read()
    ...
    ...     @skip_conditional(lambda : True):
    ...     def test_that_never_runs(self):
    ...         pass
    '''
    def __init__(self, skip_condition, message="skipped"):
        self._skip_condition = skip_condition
        self._message = message

    def __call__(self, func):
        @functools.wraps(func)
        def inner(*args, **kwargs):
            skip_condition = self._skip_condition
            if callable(skip_condition):
                # args[0] is ``self``
                skip_condition = skip_condition(args[0])

            if skip_condition:
                print("skipping %r - %s" % (func, self._message))
                return True
            return func(args[0])

        return inner


class skip_test(skip_conditional):
    ''''
    Unconditionally skip a test
    '''
    def __init__(self, skip_condition, *args, **kwargs):
        super(skip_test, self).__init__(True, *args, **kwargs)


java_only_test = lambda: skip_conditional(lambda self: not self.app_type == 'java')

cpp_only_test = lambda: skip_conditional(lambda self: not self.app_type == 'cpp')

jni_only_test = lambda: skip_conditional(lambda self: not self.app_type == 'jni')


def wimpy(func):
    '''
    Mark a test as 'wimpy' that is - a function specifically known to be quick-running.
    This implementation simply adds the `.wimpy` attribute to the decorated function
    and returns it, otherwise unmodified
    '''
    func.wimpy = True

    return func


class ordered_test(object):
    '''Set the ordered attribute on function'''
    def __init__(self, order):
        self._order = order

    def __call__(self, func):
        func.test_order = self._order
        return func


class deprecated(object):
    """
    method or function decorator used to warn of pending feature removal:

    >>> @deprecated()
    ... def myfunc():
    ...     return 'hello'
    ...
    >>> myfunc()
        DeprecationWarning: `__main__.myfunc()` is deprecated and will be removed soon.
    'hello'
    >>> class MyClass(object):
    ... @deprecated(alternative_feature='print')
    ... def myprint(self, *args, **kwargs):
    ...     print(*args, **kwargs)
    ...
    >>> obj = MyClass()
    >>> obj.myprint("hello")
    DeprecationWarning: `__main__.MyClass.myfunc()` is deprecated and will be removed soon. Use 'print' instead.
    hello
    """

    def __init__(
            self,
            alternative_feature=None,
            removal_date='soon',
            exception=UserWarning
        ):
        self.alternative_feature_message = (
            alternative_feature and 'use %r instead' % alternative_feature or ''
        )
        self.exception = exception
        self.removal_date = removal_date

    def __call__(self, func):
        class_name = ''
        if getattr(func, 'im_class', None):
            class_name = '%s.' % func.im_class.__name__

        if getattr(func, 'im_func', None):
            func_name = func.im_func.func_name
        else:
            func_name = func.func_name

        module_name = getattr(func, '__module__')

        warning = "`%s.%s%s()` is deprecated and will be removed %s. %s" % (
            module_name,
            class_name,
            func_name,
            self.removal_date,
            self.alternative_feature_message
        )

        @functools.wraps(func)
        def inner(*args, **kwargs):
            if not getattr(func, 'deprecation_warned', False):
                warnings.warn(warning, self.exception, 2)
                func.deprecation_warned = True
            return func(*args, **kwargs)

        return inner
