FROM quay.io/centos/centos:stream10 AS build

RUN yum update -y 

RUN yum groupinstall "Development Tools" -y
RUN yum install -y \
    wget tar \
    gcc gcc-c++ \
    cmake make \
    bzip2 bzip2-devel \
    zlib zlib-devel

RUN yum install -y libstdc++-devel
RUN yum install -y python3 python3-devel
RUN yum install -y iputils

RUN yum clean all

# boost

ENV BOOST_SRC_URL=https://archives.boost.io
ENV BOOST_VERSION=1_86_0
ENV BOOST_VERSION_WITH_DOTS=1.86.0

RUN wget ${BOOST_SRC_URL}/release/${BOOST_VERSION_WITH_DOTS}/source/boost_${BOOST_VERSION}.tar.gz
RUN tar -xvzf boost_${BOOST_VERSION}.tar.gz

# https://www.boost.org/doc/libs/1_87_0/tools/build/doc/html/index.html
# if add `install`, then here will be problems...
RUN cd boost_${BOOST_VERSION} && \
    ./bootstrap.sh && \
    ./b2 -j4 \
        --with-system --with-thread --with-log --with-filesystem \
        toolset=gcc threading=multi link=shared,static 
RUN rm -rf boost_${BOOST_VERSION}.tar.gz

ENV BOOST_INCLUDE_DIR=/boost_${BOOST_VERSION}
ENV BOOST_LIBRARY_DIR=/boost_${BOOST_VERSION}/stage/lib

# lua

RUN dnf --enablerepo=crb install \
    lua lua-devel -y

# from dir `dependency`

WORKDIR /wc1
COPY . .
WORKDIR /wc1/server

RUN git submodule update \
    --init --remote --recursive

RUN cd dependency/clickhouse && \
    mkdir build && cd build && \
    cmake ..

RUN cd dependency/clickhouse/build && \
    make -j4

# qmake

RUN yum install -y \
    qt6-qtbase-devel qt6-qttools-devel 

RUN yum clean all

# checks

WORKDIR /wc1/server/app 
RUN echo "ls: $(ls)"

# build app

RUN qmake CONFIG+=release
RUN make -j4

RUN echo "ls: $(ls)"

# copy 

FROM quay.io/centos/centos:stream10 

COPY --from=build /wc1/server/app/app \
                  /wc1/server/app/app

# ???
COPY --from=build /usr/lib64/liblua-5.4.so \
                  /usr/lib64/liblua-5.4.so
WORKDIR /wc1/server/app

CMD ./app
