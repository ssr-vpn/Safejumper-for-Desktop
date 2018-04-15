#-------------------------------------------------
# Safejumper sources in one project so qmake can
# create a makefile to build it all at once.
#-------------------------------------------------

TEMPLATE = subdirs

SUBDIRS = \
        safejumper \
        service

macx: {
SUBDIRS += \
    launchopenvpn \
    netdown

    # Bundle App name
    BUNDLEAPP = "Safejumper"
    APPNAME = "Safejumper.app"
    HELPERAPP = sh.proxy.SafejumperHelper
    HELPERAPP_INFO = SafejumperHelper-Info.plist
    HELPER_APP_LAUNCHD_INFO = sh.proxy.SafejumperHelper.plist
    RESOURCES_INST_DIR = $$shell_quote($$OUT_PWD/safejumper/$${APPNAME}/Contents/Resources/)

    # Commands to organize the bundle app
    organizer.commands += $(MKDIR) $$shell_quote($$OUT_PWD/safejumper/$${APPNAME}/Contents/Library/LaunchServices);
    organizer.commands += $(COPY) $$shell_quote($$OUT_PWD/$${HELPERAPP}) $$shell_quote($$OUT_PWD/safejumper/$${APPNAME}/Contents/Library/LaunchServices);
    organizer.commands += $(COPY) $$PWD/service/$${HELPERAPP_INFO} $$shell_quote($$OUT_PWD/safejumper/$${APPNAME}/Contents/Resources);
    organizer.commands += $(COPY) $$PWD/service/$${HELPER_APP_LAUNCHD_INFO} $$shell_quote($$OUT_PWD/safejumper/$${APPNAME}/Contents/Resources);

    include (common/certificate.pri)

    # Bundle identifier for your application
    BUNDLEID = sh.proxy.Safejumper

    QMAKE_CFLAGS_RELEASE = $$QMAKE_CFLAGS_RELEASE_WITH_DEBUGINFO
    QMAKE_CXXFLAGS_RELEASE = $$QMAKE_CXXFLAGS_RELEASE_WITH_DEBUGINFO
    QMAKE_OBJECTIVE_CFLAGS_RELEASE =  $$QMAKE_OBJECTIVE_CFLAGS_RELEASE_WITH_DEBUGINFO
    QMAKE_LFLAGS_RELEASE = $$QMAKE_LFLAGS_RELEASE_WITH_DEBUGINFO

    # Extract debug symbols
    # codesigner.commands += dsymutil $$shell_quote($${OUT_PWD}/$${APPNAME}/Contents/MacOS/$${BUNDLEAPP}) -o $$shell_quote($${OUT_PWD}/$${APPNAME}.dSYM);
    # codesigner.commands += $(COPY_DIR) $$shell_quote($${OUT_PWD}/$${APPNAME}.dSYM) $$shell_quote($${OUT_PWD}/$${APPNAME}/Contents/MacOS/$${APPNAME}.dSYM);

    # deploy qt dependencies
    codesigner.commands += macdeployqt $$shell_quote($${OUT_PWD}/safejumper/$${APPNAME}) -always-overwrite -codesign=$${CERTSHA1};

    # set the modification and access times of files
    codesigner.commands += touch -c $$shell_quote($${OUT_PWD}/safejumper/$${APPNAME});

    # Sign the application, using the provided entitlements
    CODESIGN_ALLOCATE_PATH=$$system(xcrun -find codesign_allocate)
    codesigner.commands += export CODESIGN_ALLOCATE=$${CODESIGN_ALLOCATE_PATH};
    codesigner.commands += codesign --force --sign $${CERTSHA1} -r=\'designated => anchor apple generic and identifier \"$${BUNDLEID}\" and ((cert leaf[field.1.2.840.113635.100.6.1.9] exists) or (certificate 1[field.1.2.840.113635.100.6.2.6] exists and certificate leaf[field.1.2.840.113635.100.6.1.13] exists and certificate leaf[subject.OU]=$${CERT_OU}))\' --timestamp=none $$shell_quote($$OUT_PWD/safejumper/$${APPNAME}) > /dev/null 2>&1;

    first.depends = $(first) organizer codesigner
    export(first.depends)
    export(organizer.commands)
    export(codesigner.commands)
    QMAKE_EXTRA_TARGETS += first organizer codesigner
}

linux: {
SUBDIRS += \
    launchopenvpn \
    netdown
}

