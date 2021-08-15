import unittest

from parser.input import MATCHER, MATCHER_FOR_OLD

class TestInput(unittest.TestCase):
  def check_match(self, line):
    m = MATCHER.match(line)
    self.assertTrue(m, "'" + line + "'")

  def check_old_match(self, line):
    m = MATCHER_FOR_OLD.match(line)
    self.assertTrue(m, "'" + line + "'")

  def test_thread_names(self):
    lines = """
      NeuralNetworks-5143  ( 5143) [005] ...1   142.924145: tracing_mark_write: B|5143|[NN_L
    <...>-5149 ( 774) [000] ...1   143.103773: tracing_mark_write: B|774|[NN_LDV_PC][validat
    <...>-756 ( 756) [000] ...1   143.140553: tracing_mark_write: B|756|HIDL::IDevice::prepa
    <...>-5149  (-----) [001] ...1   143.149856: tracing_mark_write: B|756|[NN_LCC_PE][optim
      HwBinder:784_1-5236  (  784) [001] ...1   397.528915: tracing_mark_write: B|784|HIDL::
      GLThread 35-1739  ( 1500) [001] ...1   277.001798: tracing_mark_write: B|1500|HIDL::IMapper::importBuffer::passthrough")
      allocator@1.0-s-757   (  757) [005] ...1  1700.939402: tracing_mark_write: E|757
    """
    for line in lines.splitlines():
      line = line.strip()
      if line:
        self.check_match(line)

  def test_old_systrace(self):
    lines = """
           <...>-4762  [007] .... 407453.701225: tracing_mark_write: B|4762|HIDL::IServiceManager::getTransport::client
           <...>-550   [007] .... 407453.701675: tracing_mark_write: E|550
           <...>-4840  [005] .... 407457.494359: tracing_mark_write: B|4828|[SW][NN_LI_PE]StepExecutor::startComputeOnDevice::execute
           <...>-4840  [005] .... 407457.494365: tracing_mark_write: B|4828|HIDL::IPreparedModel::execute::client
           <...>-13947 [005] .... 407457.494606: tracing_mark_write: B|13947|HIDL::IPreparedModel::execute::server
           <...>-13949 [005] .... 407457.508264: tracing_mark_write: B|13947|HIDL::IExecutionCallback::notify::client"""
    for line in lines.splitlines():
      line = line.strip()
      if line:
        self.check_old_match(line)
