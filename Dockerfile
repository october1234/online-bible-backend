FROM alpine

WORKDIR /online-bible

COPY ./app app
COPY ./frontend frontend
COPY ./Hb5_utf8_rev2 Hb5_utf8_rev2
RUN chmod 777 app

EXPOSE 80

CMD [ "/online-bible/app" ]

