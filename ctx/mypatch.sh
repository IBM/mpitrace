patchelf --replace-needed libmpi_ibm_mpifh.so.3 libmpi_ibm_mpifh.so libmpitrace.so
patchelf --replace-needed libmpiprofilesupport.so.3 libmpiprofilesupport.so libmpitrace.so
patchelf --replace-needed libmpi_ibm.so.3 libmpi_ibm.so libmpitrace.so
patchelf --remove-rpath libmpitrace.so

patchelf --replace-needed libmpi_ibm_mpifh.so.3 libmpi_ibm_mpifh.so libmpihpm.so
patchelf --replace-needed libmpiprofilesupport.so.3 libmpiprofilesupport.so libmpihpm.so
patchelf --replace-needed libmpi_ibm.so.3 libmpi_ibm.so libmpihpm.so
patchelf --remove-rpath libmpihpm.so
