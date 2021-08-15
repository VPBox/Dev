import unittest

from parser.tracker import Tracker, AppPhase
from parser.aggregate import aggregate_times

class TestStats(unittest.TestCase):
  def setUp(self):
    self.app_phase = AppPhase()
    self.tracker1 = Tracker(1, False, self.app_phase)
    self.tracker2 = Tracker(2, True, self.app_phase)
    self.timevars = dict()
    for t in range(0, 20):
      name = "t" + str(t)
      self.timevars[name] = self.from_spectime(name)

  def from_spectime(self, spectime):
      return 10 ** int(spectime[1:])

  def feed_spec_text(self, text):
    for line in text.split("\n"):
      line = line.lstrip().rstrip()
      if line == "":
        continue
      time, _, *mark = line.split(":")
      mark = ":".join(mark)
      mark = mark.replace("<thread", "").replace(">", "")
      if mark[2:3] == "1":
        self.tracker1.handle_mark(self.from_spectime(time), mark)
      else:
        self.tracker2.handle_mark(self.from_spectime(time), mark)
    self.times, self.self_times, self.has_warmup_and_benchmark, _ = aggregate_times(
        dict(t1=self.tracker1, t2=self.tracker2),
        special_case_lr_pe=False)

  def check_overall_time(self, tag, spec_interval, app_phase=None):
    expected = eval(spec_interval, {}, self.timevars) * 1000.0
    layer, phase = tag.split("_")
    if not app_phase:
      actual = self.times[phase][layer]
    else:
      actual = self.times[app_phase][phase][layer]
    self.assertAlmostEqual(expected, actual, places=1,
                           msg="\n".join([
                               self.tracker1.debugstring,
                               str(self.times)]))

  def check_self_time(self, tag, spec_interval):
    expected = eval(spec_interval, {}, self.timevars) * 1000.0
    layer, phase = tag.split("_")
    actual = self.self_times[phase][layer]
    self.assertAlmostEqual(expected, actual, places=1,
                           msg=self.tracker1.debugstring)

  def test_baseline(self):
    spec = """
    t0: tracing_mark_write:B|<thread1>|[NN_LR_PP]funcP
    t1: tracing_mark_write:E|<thread1>
    """
    self.feed_spec_text(spec)
    self.check_overall_time("LR_PP", "(t1-t0)")

  def test_local_call_to_other_layer(self):
    spec = """
    t0: tracing_mark_write:B|<thread1>|[NN_LA_PP]funcE1
    t1: tracing_mark_write:B|<thread1>|[NN_LR_PP]funcC1
    t2: tracing_mark_write:E|<thread1>
    t3: tracing_mark_write:E|<thread1>
    """
    self.feed_spec_text(spec)
    self.check_overall_time("LA_PP", "(t3-t0)")
    self.check_overall_time("LR_PP", "(t2-t1)")
    self.check_self_time("LA_PP", "(t3-t0) - (t2-t1)")

  def test_local_call_to_other_layer_wrong_nesting(self):
    spec = """
    t0: tracing_mark_write:B|<thread1>|[NN_LR_PP]funcE1
    t1: tracing_mark_write:B|<thread1>|[NN_LA_PP]funcC1
    t2: tracing_mark_write:E|<thread1>
    t3: tracing_mark_write:E|<thread1>
    """
    with self.assertRaises(Exception) as ctx:
      self.feed_spec_text(spec)

  def test_switch_phase_during_execution(self):
    spec = """
    t0: tracing_mark_write:B|<thread1>|[NN_LC_PTR]funcC1
    t1: tracing_mark_write:B|<thread1>|[SW][NN_LC_PCO]funcC1
    t2: tracing_mark_write:E|<thread1>
    t3: tracing_mark_write:E|<thread1>
    """
    self.feed_spec_text(spec)
    self.check_overall_time("LC_PTR", "(t1-t0)")
    self.check_overall_time("LC_PCO", "(t2-t1)")

  def test_subphases(self):
    spec = """
    t0: tracing_mark_write:B|<thread1>|[NN_LR_PE]funcR2
    t1: tracing_mark_write:B|<thread1>|[NN_LC_PCO]funcC2
    t2: tracing_mark_write:E|<thread1>
    t3: tracing_mark_write:E|<thread1>
    """
    self.feed_spec_text(spec)
    self.check_overall_time("LC_PCO", "(t2-t1)")
    self.check_overall_time("LC_PE", "(t2-t1)")
    self.check_overall_time("LR_PE", "(t3-t0)")
    self.check_self_time("LC_PCO", "(t2-t1)")
    self.check_self_time("LR_PE", "(t3-t0)-(t2-t1)")

  def test_subphases_nested(self):
    spec = """
    t0: tracing_mark_write:B|<thread1>|[NN_LR_PE]funcR2
    t1: tracing_mark_write:B|<thread1>|[NN_LC_PCO]funcC2
    t2: tracing_mark_write:B|<thread1>|[NN_LC_PCO]funcC2A
    t3: tracing_mark_write:E|<thread1>
    t4: tracing_mark_write:E|<thread1>
    t5: tracing_mark_write:E|<thread1>
    """
    self.feed_spec_text(spec)
    self.check_overall_time("LC_PCO", "(t4-t1)")
    self.check_overall_time("LC_PE", "(t4-t1)")
    self.check_overall_time("LR_PE", "(t5-t0)")
    self.check_self_time("LR_PE", "(t5-t0)-(t4-t1)")

  def test_subphases_la_pr(self):
    spec = """
    t0: tracing_mark_write:B|<thread1>|[NN_LA_PE]lape
    t1: tracing_mark_write:B|<thread1>|[NN_LA_PR]lapr
    t2: tracing_mark_write:E|<thread1>
    t3: tracing_mark_write:E|<thread1>
    """
    self.feed_spec_text(spec)
    self.check_overall_time("LA_PE", "(t3-t0)")
    self.check_overall_time("LA_PR", "(t2-t1)")
    self.check_self_time("LA_PE", "(t3-t0)")
    self.check_self_time("LA_PR", "(t2-t1)")

  def test_subphases_and_layers(self):
    spec = """
    t0: tracing_mark_write:B|<thread1>|[NN_LA_PE]pe
    t1: tracing_mark_write:B|<thread1>|[NN_LR_PE]pe
    t2: tracing_mark_write:B|<thread1>|[NN_LR_PIO]pio
    t3: tracing_mark_write:B|<thread1>|[NN_LI_PIO]pio
    t4: tracing_mark_write:E|<thread1>
    t5: tracing_mark_write:E|<thread1>
    t6: tracing_mark_write:E|<thread1>
    t7: tracing_mark_write:E|<thread1>
    """
    self.feed_spec_text(spec)
    self.check_overall_time("LR_PIO", "(t5-t2)")
    self.check_overall_time("LI_PIO", "(t4-t3)")
    self.check_overall_time("LI_PE", "(t4-t3)")
    self.check_overall_time("LA_PE", "(t7-t0)")
    self.check_overall_time("LR_PE", "(t6-t1)")

  def test_additional_detail_in_same_layer(self):
    spec = """
    t0: tracing_mark_write:B|<thread1>|[NN_LR_PE]funcR3
    t1: tracing_mark_write:B|<thread1>|[NN_LR_PE]funcR4
    t2: tracing_mark_write:E|<thread1>
    t3: tracing_mark_write:E|<thread1>
    """
    self.feed_spec_text(spec)
    self.check_overall_time("LR_PE", "(t3-t0)")
    self.check_self_time("LR_PE", "(t3-t0)")

  def test_synchronous_ipc_call(self):
    spec = """
    t0: tracing_mark_write:B|<thread1>|[NN_LR_PC]funcR5
    t1: tracing_mark_write:B|<thread1>|[NN_LI_PI]getCapabilities
    t2: tracing_mark_write:B|<thread1>|HIDL::IDevice::getCapabilities::client
    t3: tracing_mark_write:E|<thread1>
    t4: tracing_mark_write:E|<thread1>
    t5: tracing_mark_write:E|<thread1>
    """
    self.feed_spec_text(spec)
    self.check_overall_time("LR_PC", "(t5-t0)-(t4-t1)")
    self.check_overall_time("LI_PI", "(t4-t1)")
    self.check_self_time("LR_PC", "(t5-t0)-(t4-t1)")

  def test_async_ipc_call_waited_synchronously(self):
    spec = """
    t0: tracing_mark_write:B|<thread1>|[NN_LI_PC]prepareModel
    t1: tracing_mark_write:B|<thread1>|HIDL::IDevice::prepareModel_1_1::client
    t2: tracing_mark_write:B|<thread2>|HIDL::IDevice::prepareModel_1_1::server
    t3: tracing_mark_write:B|<thread2>|[NN_LD_PC]SampleDriver::prepareModel
    t4: tracing_mark_write:B|<thread2>|HIDL::IPreparedModelCallback::notify::client
    t5: tracing_mark_write:E|<thread2>
    t5: tracing_mark_write:E|<thread2>
    t6: tracing_mark_write:E|<thread2>
    t7: tracing_mark_write:B|<thread1>|HIDL::IPreparedModelCallback::notify::server
    t8: tracing_mark_write:E|<thread1>
    t9: tracing_mark_write:E|<thread1>
    t10: tracing_mark_write:E|<thread1>
    """
    self.feed_spec_text(spec)
    self.check_overall_time("LI_PC", "(t10-t0)")
    self.check_overall_time("LD_PC", "(t6-t2)")

  def test_subtracting_time_direct_descendant(self):
    spec = """
    t0: tracing_mark_write:B|<thread1>|[NN_LI_PC]prepareModel
    t1: tracing_mark_write:B|<thread1>|[SUB][NN_LR_PC]VersionedIDevice::prepareM
    t2: tracing_mark_write:E|<thread1>
    t3: tracing_mark_write:E|<thread1>
    """
    self.feed_spec_text(spec)
    self.check_overall_time("LI_PC", "(t3-t0)-(t2-t1)")
    self.check_overall_time("LR_PC", "(t2-t1)")

  def test_subtracting_time_nested(self):
    spec = """
    t0: tracing_mark_write:B|<thread1>|[NN_LI_PC]prepareModel
    t1: tracing_mark_write:B|<thread1>|[NN_LI_PC]foo
    t2: tracing_mark_write:B|<thread1>|[SUB][NN_LR_PC]VersionedIDevice::prepareM
    t3: tracing_mark_write:E|<thread1>
    t4: tracing_mark_write:E|<thread1>
    t5: tracing_mark_write:E|<thread1>
    """
    self.feed_spec_text(spec)
    self.check_overall_time("LI_PC", "(t5-t0)-(t3-t2)")
    self.check_overall_time("LR_PC", "(t3-t2)")


  def test_onetime_initialization_simple(self):
    spec = """
    t0: tracing_mark_write:B|<thread1>|[NN_LR_PP]funcR5
    t1: tracing_mark_write:B|<thread1>|[NN_LR_PI]funcI
    t2: tracing_mark_write:E|<thread1>
    t3: tracing_mark_write:E|<thread1>
    """
    self.feed_spec_text(spec)
    self.check_overall_time("LR_PI", "(t2-t1)")
    self.check_overall_time("LR_PP", "(t3-t0)-(t2-t1)")

  def test_utility_code(self):
    spec = """
    t0: tracing_mark_write:B|<thread1>|[NN_LR_PP]funcR6
    t1: tracing_mark_write:B|<thread1>|[NN_LU_PU]funcU
    t2: tracing_mark_write:E|<thread1>
    t3: tracing_mark_write:E|<thread1>
    """
    self.feed_spec_text(spec)
    self.check_overall_time("LR_PP", "(t3-t0)")

  def test_app_layer(self):
    spec = """
    t0: tracing_mark_write:B|<thread1>|[NN_LA_PO]po
    t1: tracing_mark_write:B|<thread1>|[NN_LA_PP]pp
    t2: tracing_mark_write:B|<thread1>|[SW][NN_LA_PC]pc
    t3: tracing_mark_write:E|<thread1>
    t4: tracing_mark_write:E|<thread1>
    t5: tracing_mark_write:E|<thread1>
    """
    self.feed_spec_text(spec)
    self.check_overall_time("LA_PO", "(t5-t0)")
    self.check_overall_time("LA_PP", "(t2-t1)")
    self.check_overall_time("LA_PC", "(t3-t2)")

  def test_app_not_using_PT(self):
    spec = """
    t0: tracing_mark_write:B|<thread1>|[NN_LA_PC]pc
    t1: tracing_mark_write:B|<thread1>|[NN_LR_PC]pc
    t2: tracing_mark_write:E|<thread1>
    t3: tracing_mark_write:B|<thread1>|[NN_LR_PT]pt
    t4: tracing_mark_write:E|<thread1>
    t5: tracing_mark_write:E|<thread1>
    """
    self.feed_spec_text(spec)
    self.check_overall_time("LA_PC", "(t5-t0)-(t4-t3)")
    self.check_overall_time("LR_PC", "(t2-t1)")
    self.check_overall_time("LR_PT", "(t4-t3)")

  def test_warmup_and_benchmark(self):
    spec = """
    t0: tracing_mark_write:B|<thread1>|[NN_LA_PWU]pwu
    t1: tracing_mark_write:B|<thread1>|[NN_LR_PE]ANeuralNetworksExecution_create
    t2: tracing_mark_write:E|<thread1>
    t3: tracing_mark_write:E|<thread1>
    t4: tracing_mark_write:B|<thread1>|[NN_LA_PBM]pbm
    t5: tracing_mark_write:B|<thread1>|[NN_LR_PE]ANeuralNetworksExecution_create
    t6: tracing_mark_write:E|<thread1>
    t7: tracing_mark_write:E|<thread1>
    """
    self.feed_spec_text(spec)
    self.check_overall_time("LR_PE", "(t2-t1)+(t6-t5)")
    self.check_overall_time("LR_PE", "(t2-t1)", "PWU")
    self.check_overall_time("LR_PE", "(t6-t5)", "PBM")
