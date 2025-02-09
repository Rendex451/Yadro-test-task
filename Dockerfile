FROM gcc:11.3.0

WORKDIR /app

COPY src/ src/
COPY Makefile run.sh ./

RUN apt update && apt install -y make gcc \
	&& make

RUN chmod +x run.sh

CMD ["sh", "-c", "./run.sh && exec /bin/bash"]