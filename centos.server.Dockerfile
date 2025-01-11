FROM quay.io/centos/centos:stream10 AS build

RUN yum update -y 
RUN yum groupinstall "Development Tools" -y
RUN  yum install -y \
        gcc-c++ \
        make \
        wget \
        tar \
        cmake && \
    yum clean all

RUN echo "dns_timeout = 5" >> /root/.wgetrc && \
    echo "timeout = 5" >> /root/.wgetrc && \
    echo "tries = 5" >> /root/.wgetrc

RUN cat /root/.wgetrc

# boost

ENV BOOST_VERSION=1.86.0

RUN wget https://github.com/boostorg/boost/releases/download/boost-${BOOST_VERSION}/boost-${BOOST_VERSION}-cmake.tar.gz
RUN echo "ls: $(ls)"

RUN tar -xvzf boost-${BOOST_VERSION}-cmake.tar.gz 
RUN echo "ls: $(ls)"

RUN cd boost-${BOOST_VERSION} && \
    ./bootstrap.sh && \
    ./b2 --with-system --with-thread --with-log --with-filesystem \
        toolset=gcc threading=multi link=shared,static --prefix=/usr/local && \
    cd ..
RUN rm -rf boost-${BOOST_VERSION}.tar.gz

ENV BOOST_INCLUDE_DIR=/boost-${BOOST_VERSION}
ENV BOOST_LIBRARY_DIR=/boost-${BOOST_VERSION}/stage/lib

RUN echo "BOOST_INCLUDE_DIR=${BOOST_INCLUDE_DIR}" && \
    echo "BOOST_LIBRARY_DIR=${BOOST_LIBRARY_DIR}"

# lua

RUN dnf --enablerepo=crb install \
    lua lua-devel -y

# from dir `dependency`

WORKDIR /wc1
COPY . .
WORKDIR /wc1/server

RUN git submodule update --init --remote --recursive
RUN cd dependency/clickhouse && \
    mkdir build && cd build && \
    cmake ..
RUN cd dependency/clickhouse/build && \
    make

# qmake

RUN yum install qt6-qtbase-devel qt6-qttools-devel -y

WORKDIR /wc1/server/app 
RUN echo "ls: $(ls)"

RUN qmake CONFIG+=release
RUN make
RUN echo "ls: $(ls)"

# copy 

FROM quay.io/centos/centos:stream10 

COPY --from=build /wc1/server/app/app /wc1/server/app/app

# ???
COPY --from=build /usr/lib64/liblua-5.4.so /usr/lib64/liblua-5.4.so
WORKDIR /wc1/server/app

CMD ./app
