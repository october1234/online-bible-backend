FROM alpine

COPY ./app /online-bible/app
COPY ./frontend /online-bible/frontend
RUN chmod 777 /online-bible/app

EXPOSE 80

CMD [ "/online-bible/app" ]

