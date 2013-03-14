#git:slp/pkgs/v/vmodem-daemon-emulator
Name: vmodemd-emul
Version: 0.2.40
Release: 1
Summary: Modem Emulator
Group: System/ModemEmulator
License: GNU
Source0: %{name}-%{version}.tar.gz
Source1001: packaging/vmodemd-emul.manifest
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

if [ ! -d %{buildroot}/usr/lib/systemd/system/multi-user.target.wants ]; then
    mkdir -p %{buildroot}/usr/lib/systemd/system/multi-user.target.wants
fi
cp vmodem-x86.service %{buildroot}/usr/lib/systemd/system/.
ln -s ../vmodem-x86.service %{buildroot}/usr/lib/systemd/system/multi-user.target.wants/vmodem-x86.service

%clean
make clean
rm -f build

%post

%postun

%files
%defattr(-,root,root,-)
%attr(755,-,-) %{_bindir}/vmodem_x86
/usr/lib/systemd/system/multi-user.target.wants/vmodem-x86.service
/usr/lib/systemd/system/vmodem-x86.service

%changelog


