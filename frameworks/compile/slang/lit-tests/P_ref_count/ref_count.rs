// RUN: %Slang %s
// RUN: %rs-filecheck-wrapper %s

#pragma version(1)
#pragma rs java_package_name(ref_count)

struct Wrapper {
  rs_element e;
  rs_type t;
};


// CHECK: call void @_Z13rsClearObjectP10rs_element(%struct.rs_element{{.*}}* nonnull %.rs.tmp{{[0-9]+}})
// CHECK: call void @_Z11rsSetObjectP10rs_elementS_(%struct.rs_element{{.*}}* nonnull %.rs.retval{{[0-9]+}}, {{.*}})

static rs_element bar() {
  rs_element x = {0};
  return x;
}

// CHECK: call void @_Z11rsSetObjectP10rs_elementS_(%struct.rs_element{{.*}}* %{{.*}}, {{.*}})
// CHECK: call void @_Z11rsSetObjectP7rs_typeS_(%struct.rs_type{{.*}}* %{{.*}}, {{.*}})
// CHECK: call void @_Z13rsClearObjectP10rs_element(%struct.rs_element{{.*}}* {{.*}})
// CHECK: call void @_Z13rsClearObjectP7rs_type(%struct.rs_type{{.*}}* {{.*}})

static struct Wrapper baz() {
  struct Wrapper x = { 0, 0 };
  return x;
}

void entrypoint() {
  rs_element e = bar();
  struct Wrapper w = baz();
  if (rsIsObject(e) && rsIsObject(w.e) && rsIsObject(w.t)) {
    rsDebug("good objects", 0);
  }
}
