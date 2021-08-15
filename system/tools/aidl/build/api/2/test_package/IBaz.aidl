package test_package;
interface IBaz {
  oneway void CanYouDealWithThisBar(in some_package.IBar bar);
  void MethodAddedInVersion2();
  ParcelFileDescriptor readPFD();
  void writePFD(in ParcelFileDescriptor fd);
  void readWritePFD(inout ParcelFileDescriptor fd);
}
