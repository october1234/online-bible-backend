FROM alpine

WORKDIR /online-bible

COPY ./app app
COPY ./frontend frontend
COPY ./Hb5_utf8_rev2.sqlite Hb5_utf8_rev2.sqlite
RUN chmod 777 app

EXPOSE 80

CMD [ "/online-bible/app" ]

