#git:slp/pkgs/v/vmodem-daemon-emulator
Name: vmodemd-emul
Version: 0.2.18
Release: 1
Summary: Modem Emulator
Group: System/ModemEmulator
License: GNU
Source0: %{name}-%{version}.tar.gz
ExclusiveArch: %{ix86}
BuildRequires: pkgconfig(glib-2.0)
BuildRequires: pkgconfig(vconf)
BuildRequires: pkgconfig(sqlite3)

%description
Virtual modem emulator

%prep
%setup -q

%build
autoconf
%configure

make %{?jobs:-j%jobs}

%install
rm -rf %{buildroot}
mkdir -p %{buildroot}/usr/bin
make install prefix=%{buildroot}

%clean
make clean
rm -f build

%post

%postun

%files
%defattr(-,root,root,-)
%attr(755,-,-) %{_bindir}/vmodem_x86

%changelog


