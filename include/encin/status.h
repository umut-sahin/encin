#pragma once

#define encin_status (*encin_status_pointer())

encin_result *encin_status_pointer();

const char *encin_status_description();
