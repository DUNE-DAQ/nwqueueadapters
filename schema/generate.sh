#!/usr/bin/env bash

# Really ugly and temporary glue to run moo code generator.
# This will simplify and move into CMake.

# For current best ideas for CMake integration see:
# https://brettviren.github.io/moo/buildsys.html#cmake

# For guidance on how to write schema see:
# https://brettviren.github.io/moo/dunedaq-appfwk-schema.html

# For guidance on how to make schema objects see:
# https://brettviren.github.io/dune-daq-repl/ddcmd.html

# For guidance on how to deliver objects to daq_application see:
# https://brettviren.github.io/dune-daq-repl/ddrepl.html

mydir=$(dirname $(realpath $BASH_SOURCE))
srcdir=$(dirname $mydir)

# Wrap up the render command.  This bakes in a mapping to file name
# which would be better somehow captured by the schema itself.
render () {
    local name="$1" ; shift
    local What="$1" ; shift
    local outdir="${1:-$srcdir/include/networkqueue/${name}}"; shift
    local hppcpp="hpp"
    if [ -n "$1" ]; then
        hppcpp="$1"; shift
    fi
    
    local what="$(echo $What | tr '[:upper:]' '[:lower:]')"
    local tmpl="networkqueue-${what}.${hppcpp}.j2"
    local outhpp="$outdir/${What}.${hppcpp}"
    mkdir -p $outdir
    set -x
    moo -g '/lang:ocpp.jsonnet' \
        -M $mydir -T $mydir \
        -M /home/rodrigues/dune/daq/appfwk-network-queue/sourcecode/moo/moo/jsonnet-code \
        -A path="dunedaq.networkqueue.${name}" \
        -A ctxpath="dunedaq" \
        -A os="networkqueue-${name}-schema.jsonnet" \
        render networkqueue-model.jsonnet $tmpl \
        > $outhpp || exit -1
    set +x
    echo $outhpp
}


render nq Structs
render nq Nljs

render fsd Structs   $srcdir/test/include/networkqueue/fsd
render fsd Nljs      $srcdir/test/include/networkqueue/fsd
render fsd NToQ      $srcdir/test/include/networkqueue/fsd  cpp
# TODO: We're manually setting the output name of the queue adapter
# specialization files here, because cetlib requires that the filename
# matches the class name. But the class name here is typedef'ed, with
# the typedef being defined within the template that moo uses. Somehow
# this all has to become automatic. There's also the problem that the
# template that generates the typedef file will make a typedef for
# every type in the schema file. But cetlib requires that there's only
# *one* DAQModule defined in each library, so this will break when
# there's more than one type in the schema file
mv "${srcdir}/test/include/networkqueue/fsd/NToQ.cpp" "${srcdir}/test/plugins/NetworkToQueueAdapterDAQModuleOfFakeData.cpp"
render fsd QToN      $srcdir/test/include/networkqueue/fsd  cpp
mv "${srcdir}/test/include/networkqueue/fsd/QToN.cpp" "${srcdir}/test/plugins/QueueToNetworkAdapterDAQModuleOfFakeData.cpp"

render fsdp Structs  $srcdir/test/include/networkqueue/fsdp
render fsdp Nljs     $srcdir/test/include/networkqueue/fsdp

render fsdc Structs  $srcdir/test/include/networkqueue/fsdc
render fsdc Nljs     $srcdir/test/include/networkqueue/fsdc


