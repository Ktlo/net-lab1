FROM alpine:latest AS base
RUN [ "apk", "--no-cache", "--no-progress", "add", "libstdc++", "rlwrap" ]

FROM base AS build
RUN apk --no-cache --no-progress add build-base ninja coreutils cmake git python3 py-pip \
    && pip3 install meson
ADD . /lab1-chat
WORKDIR /lab1-chat
RUN meson -Dprefix=`pwd`/out -Dbuildtype=release -Ddefault_library=static -Doptimization=3 build \
    && cd build && ninja && ninja install

FROM base
COPY --from=build /lab1-chat/out/bin/lab1-chat-server /lab1-chat/out/bin/lab1-chat-client /usr/local/bin/
COPY /entrypoint.sh /entrypoint
LABEL maintainer="ktlo <ktlo@handtruth.com>"
ENTRYPOINT [ "/entrypoint" ]
