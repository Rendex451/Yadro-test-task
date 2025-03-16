FROM gcc:11.3.0

WORKDIR /app

COPY src/ src/
COPY --chmod=0100 run.sh ./
COPY Makefile ./

RUN apt update && apt install -y make gcc

RUN make

CMD ["sh", "-c", "./run.sh && exec /bin/bash"]