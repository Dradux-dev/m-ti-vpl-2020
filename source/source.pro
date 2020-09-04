TEMPLATE = subdirs

SUBDIRS += \
    benchmarks \
    demos \
    dependencies \
    libs \
    projects \
    tests \
    tools

libs.depend = dependencies
tools.depend = dependencies
benchmarks.depend = dependencies libs tools
demos.depend = dependencies libs tools
tests.depend = dependencies libs tools
projects.depend = dependencies libs tools tests
