FROM python:3.10-rc
COPY ./config.json /Code/config.json
COPY ./Code /Code
COPY ./BD /BD
WORKDIR /Code
RUN pip3 install -r requirements.txt
CMD python3 main.py