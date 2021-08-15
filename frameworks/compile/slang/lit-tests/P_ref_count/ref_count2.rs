// RUN: %Slang %s
// RUN: %rs-filecheck-wrapper %s

#pragma version(1)
#pragma rs java_package_name(ref_count2)

// CHECK: %[[RETVAL:[A-Za-z][A-Za-z0-9]*]] = call i32 @_Z18rsGetElementAt_int13rs_allocationj{{.*}}
// CHECK: call void @_Z13rsClearObjectP13rs_allocation(%struct.rs_allocation{{.*}}* {{.*}})
// CHECK: ret i32 %[[RETVAL]]
static int goo(rs_allocation a) {
  return rsGetElementAt_int(a, 0);
}

void entrypoint() {
  rs_allocation a = rsCreateAllocation_int(100);
  rsDebug("val at 0:", goo(a));
}
