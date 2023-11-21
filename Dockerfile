FROM scratch

COPY ./app /app
COPY ./frontend /frontend

RUN chmod +x /app

EXPOSE 80

CMD [ "./app" ]

