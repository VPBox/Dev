// RUN: %Slang %s

// RUN: %rs-filecheck-wrapper %s --check-prefix=CHECK-SET
//     Check rsSetObject() calls are properly added.

// CHECK-SET-NOT: call void @_Z11rsSetObject{{.*}}({{.*}} %.rs.param.a{{[02]}}, {{.*}})
// CHECK-SET:     call void @_Z11rsSetObjectP10rs_elementS_(%struct.rs_element{{.*}}* nonnull %.rs.param.a1, {{.*}})
// CHECK-SET-NOT: call void @_Z11rsSetObject{{.*}}({{.*}} %.rs.param.a{{[02]}}, {{.*}})

// CHECK-SET:    %[[F1:[A-Za-z][A-Za-z0-9]*]] = getelementptr inbounds %struct.Wrapper{{.*}}, %struct.Wrapper{{.*}}* %.rs.param.a3, i{{(32|64)}} 0, i32 0
// CHECK-SET-NOT: call void @_Z11rsSetObject{{.*}}({{.*}} %.rs.param.a{{[02]}}, {{.*}})
// CHECK-SET:     call void @_Z11rsSetObjectP13rs_allocationS_(%struct.rs_allocation{{.*}}* %[[F1]], {{.*}})
// CHECK-SET-NOT: call void @_Z11rsSetObject{{.*}}({{.*}} %.rs.param.a{{[02]}}, {{.*}})
// CHECK-SET:    %[[F2:[A-Za-z][A-Za-z0-9]*]] = getelementptr inbounds %struct.Wrapper{{.*}}, %struct.Wrapper{{.*}}* %.rs.param.a3, i{{(32|64)}} 0, i32 1
// CHECK-SET-NOT: call void @_Z11rsSetObject{{.*}}({{.*}} %.rs.param.a{{[02]}}, {{.*}})
// CHECK-SET:     call void @_Z11rsSetObjectP7rs_typeS_(%struct.rs_type{{.*}}* %[[F2]], {{.*}})
// CHECK-SET-NOT: call void @_Z11rsSetObject{{.*}}({{.*}} %.rs.param.a{{[02]}}, {{.*}})

// RUN: %rs-filecheck-wrapper %s --check-prefix=CHECK-CLR
//     Check rsClearObject() calls are properly added.

// CHECK-CLR-DAG:    %[[F10:[A-Za-z][A-Za-z0-9\.]*]] = getelementptr inbounds %struct.Wrapper{{.*}}, %struct.Wrapper{{.*}}* %a3, i{{(32|64)}} 0, i32 0
// CHECK-CLR-DAG:    %[[F20:[A-Za-z][A-Za-z0-9\.]*]] = getelementptr inbounds %struct.Wrapper{{.*}}, %struct.Wrapper{{.*}}* %a3, i{{(32|64)}} 0, i32 1

// CHECK-CLR-NOT: call void @_Z13rsClearObject{{.*}}({{.*}}* %.rs.param.a{{[02]}})
// CHECK-CLR:     call void @_Z13rsClearObjectP10rs_element(%struct.rs_element{{.*}}* nonnull %x)
// CHECK-CLR-NOT: call void @_Z13rsClearObject{{.*}}({{.*}}* %.rs.param.a{{[02]}})
// CHECK-CLR:     call void @_Z13rsClearObjectP10rs_element(%struct.rs_element{{.*}}* {{.*}}%a1)
// CHECK-CLR-NOT: call void @_Z13rsClearObject{{.*}}({{.*}}* %.rs.param.a{{[02]}})
// CHECK-CLR:     call void @_Z13rsClearObjectP13rs_allocation(%struct.rs_allocation{{.*}}* %[[F10]])
// CHECK-CLR-NOT: call void @_Z13rsClearObject{{.*}}({{.*}}* %.rs.param.a{{[02]}})
// CHECK-CLR:     call void @_Z13rsClearObjectP7rs_type(%struct.rs_type{{.*}}* %[[F20]])
// CHECK-CLR-NOT: call void @_Z13rsClearObject{{.*}}({{.*}}* %.rs.param.a{{[02]}})

#pragma version(1)
#pragma rs java_package_name(ref_count)

static int g0;
static float g2;

struct Wrapper {
  rs_allocation f1;
  rs_type f2;
  int f3;
};

static void bar(int a0, rs_element a1, float a2, struct Wrapper a3) {
  rs_element x = {0};
  struct Wrapper y = {0};
  g0 = a0;
  a1 = x;
  g2 = a2;
  a3 = y;
}

void entrypoint() {
  rs_element e = {0};
  struct Wrapper w = {0};
  bar(3, e, 2.718f, w);
  if (rsIsObject(e) && rsIsObject(w.f1) && rsIsObject(w.f2)) {
    rsDebug("good objects", 0);
  }
}


