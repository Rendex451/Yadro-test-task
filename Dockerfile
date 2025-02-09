FROM gcc:11.3.0

WORKDIR /app

COPY /src/shm_writer.c /src/shm_reader.c run.sh /app/

RUN gcc shm_writer.c -o writer && gcc shm_reader.c -o reader && chmod +x run.sh

CMD ["sh", "-c", "./run.sh && exec /bin/bash"]