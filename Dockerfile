FROM scratch

COPY ./app /app
COPY ./frontend /frontend

EXPOSE 80

ENTRYPOINT [ "/app" ]

