This folder contains a modified version of the FlatBuffers implementation header
file (flatbuffers.h) which customizes it for running in the CHRE environment.
When upgrading to a newer FlatBuffers release, be sure to manually merge the
changes described in the comment at the top of flatbuffers.h, and apply them to
new additions as well (e.g. removal of std::string usage) to maintain support.
The FlatBuffers IDL compiler (flatc) can be used without modification.

The FlatBuffers project is hosted at https://github.com/google/flatbuffers/
