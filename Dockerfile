FROM alpine

WORKDIR /online-bible

COPY ./app app
COPY ./frontend frontend
RUN chmod 777 app

EXPOSE 80

CMD [ "app" ]

