#pragma version(1)
#pragma rs java_package_name(foo)

void root(const void *ain, void *aout, const void *usrData,
          uint32_t x, uint32_t y) {
}

void in_only(const void *ain) {
}

void in_x_only(const void *ain, uint32_t x) {
}

void in_y_only(const void *ain, uint32_t y) {
}

void in_x_y_only(const void *ain, uint32_t x, uint32_t y) {
}

void in_usrdata_only(const void *ain, const void *usrData) {
}

void in_usrdata_x_only(const void *ain, const void *usrData, uint32_t x) {
}

void in_usrdata_y_only(const void *ain, const void *usrData, uint32_t y) {
}

void in_usrdata_x_y_only(const void *ain, const void *usrData, uint32_t x,
                         uint32_t y) {
}

void out_only(void *aout) {
}

void out_x_only(void *aout, uint32_t x) {
}

void out_y_only(void *aout, uint32_t y) {
}

void out_x_y_only(void *aout, uint32_t x, uint32_t y) {
}

void out_usrdata_only(void *aout, const void *usrData) {
}

void out_usrdata_x_only(void *aout, const void *usrData, uint32_t x) {
}

void out_usrdata_y_only(void *aout, const void *usrData, uint32_t y) {
}

void out_usrdata_x_y_only(void *aout, const void *usrData, uint32_t x,
                         uint32_t y) {
}

void in_out_only(const void *ain, void *aout) {
}

void in_out_x_only(const void *ain, void *aout, uint32_t x) {
}

void in_out_y_only(const void *ain, void *aout, uint32_t y) {
}

void in_out_x_y_only(const void *ain, void *aout, uint32_t x, uint32_t y) {
}

void in_out_usrdata_only(const void *ain, void *aout, const void *usrData) {
}

void in_out_usrdata_x_only(const void *ain, void *aout, const void *usrData,
                           uint32_t x) {
}

void in_out_usrdata_y_only(const void *ain, void *aout, const void *usrData,
                           uint32_t y) {
}

void in_out_usrdata_x_y_only(const void *ain, void *aout, const void *usrData,
                             uint32_t x, uint32_t y) {
}
