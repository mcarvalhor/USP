import os
import smtplib
from email import encoders
from email.mime.base import MIMEBase
from email.mime.multipart import MIMEMultipart

'''
python_3_email_with_attachment.py
Created by Robert Dempsey on 12/6/14.
Copyright (c) 2014 Robert Dempsey. Use at your own peril.
This script works with Python 3.x
NOTE: replace values in ALL CAPS with your own values
'''

# Modified by Matheus (github.com/mcarvalhor)


COMMASPACE = ', '


server			= 'smtp.sendgrid.net'
port			= 587
user			= 'USUARIO_SMTP'
password		= 'SENHA_SMTP'
recipients		= ['SEU_EMAIL']
attachments		= [input().rstrip(), input().rstrip()] # Arquivos anexos ao e-mail



outer = MIMEMultipart()
outer['Subject'] = 'Imagens run.codes'
outer['To'] = COMMASPACE.join(recipients)
outer['From'] = sender
outer.preamble = 'You will not see this in a MIME-aware mail reader.\n'

# Adicionar anexos ao e-mail
for file in attachments:
    try:
        with open(file, 'rb') as fp:
            msg = MIMEBase('application', "octet-stream")
            msg.set_payload(fp.read())
        encoders.encode_base64(msg)
        msg.add_header('Content-Disposition', 'attachment', filename=os.path.basename(file))
        outer.attach(msg)
    except:
        print("Erro ao processar anexo: ", sys.exc_info()[0])
        raise

composed = outer.as_string()

# Enviar
try:
    with smtplib.SMTP(server, port) as s:
        s.ehlo()
        s.starttls()
        s.ehlo()
        s.login(user, password)
        s.sendmail(sender, recipients, composed)
        s.close()
    print("E-mail enviado com os anexos com sucesso.")
except:
    print("Erro ao enviar o e-mail: ", sys.exc_info()[0])
    raise

