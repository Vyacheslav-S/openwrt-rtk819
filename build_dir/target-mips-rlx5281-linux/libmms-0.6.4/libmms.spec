Summary:        mms stream protocol library
Name:           libmms
Version:        0.6.4
Release:        1
License:      	LGPL
Group:          Libraries/Multimedia
Source:         libmms-%{version}.tar.gz
BuildRoot:      %{_tmppath}/%{name}-%{PACKAGE_VERSION}-root
URL:            http://libmms.sf.net

%description
libmms is a library implementing the mms streaming protocol

%package devel
Summary: Libraries and includefiles for developing with libmms
Group:	 Development/Libraries

%description devel
This package provides the necessary development headers and libraries
to allow you to devel with libmms

%prep
%setup

%build
%configure
make

%install
rm -rf $RPM_BUILD_ROOT

%makeinstall

# Clean out files that should not be part of the rpm.
# This is the recommended way of dealing with it for RH8
rm -f $RPM_BUILD_ROOT%{_libdir}/*.a
rm -f $RPM_BUILD_ROOT%{_libdir}/*.la

%clean
rm -rf $RPM_BUILD_ROOT

%post
/sbin/ldconfig

%postun
/sbin/ldconfig

%files
%defattr(-, root, root)
%doc AUTHORS COPYING.LIB ChangeLog NEWS README TODO README.LICENSE
%{_libdir}/libmms.so.*

%files devel
%{_includedir}/libmms/mms.h
%{_includedir}/libmms/bswap.h
%{_includedir}/libmms/mmsio.h
%{_includedir}/libmms/mmsh.h
%{_libdir}/libmms.so
%{_libdir}/pkgconfig/libmms.pc

%changelog
* Thu Dec 9 2004 Christian Schaller <christian@fluendo.com>
-first attempt at SPEC
