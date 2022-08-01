#!/usr/bin/env python3

""" server.py - Echo server for sending/receiving C-like structs via socket
References:
- Ctypes: https://docs.python.org/3/library/ctypes.html
- Sockets: https://docs.python.org/3/library/socket.html
"""

import socket
import sys
import random
from ctypes import *

"""connection to database"""
import pyodbc 



def main():
    PORT = 2300
    server_addr = ('localhost', PORT)
    ssock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    print("Socket created")

    try:
        # bind the server socket and listen
        ssock.bind(server_addr)
        print("Bind done")
        ssock.listen(3)
        print("Server listening on port {:d}".format(PORT))
        
        """connection to database"""
        try:
            cnxn = pyodbc.connect('Driver={FreeTDS};Server=vsbdd.database.windows.net,1433;Database=FEEDBACK_BOX_TEST;UID=fullaccess;PWD=k936zRJfae3F!;Encrypt=yes;TrustServerCertificate=no;Connection Timeout=30')
            cursor = cnxn.cursor()
        except pyodbc.Error as err:
            #print(err)
            print("vérifie ta connexion internet")
            ssock.close()
            return(0)
        
        """reception from client"""
        while True:
            csock, client_address = ssock.accept()
            print("Accepted connection from {:s}".format(client_address[0]))

            buff = csock.recv(256)
            while buff:
                print("\nReceived {:d} bytes".format(len(buff)))
                try:
                    strMsg = buff.decode()
                    paramBoxID = strMsg.split(',')[0]
                    question = strMsg.split(',')[1]
                    answer = strMsg.split(',')[2]
                    seqNb = strMsg.split(',')[3]
                    print(paramBoxID, question, answer, seqNb)
                    # send data to database
                    query = "INSERT INTO dbo.DataTest (SampleNb, ParamBoxID, QuestionTest, AnswerTest) VALUES ("+str(seqNb)+","+str(paramBoxID)+","+str(question)+","+str(answer)+");"
                    
                    try:
                        cursor.execute(query)
                    except pyodbc.Error as err:
                        print(err)
                        ssock.close()
                        return(0)
                        
                    cursor.commit()
                    
                except UnicodeDecodeError as err:
                    print(err)
                
                buff = csock.recv(256)

            print("closing connection to database")
            cursor.close()
            cnxn.close()


            print("Closing connection to client")
            print("----------------------------")
            csock.close() 

    except AttributeError as ae:
        print("Error creating the socket: {}".format(ae))
    except socket.error as se:
        print("Exception on socket: {}".format(se))
    except KeyboardInterrupt:
        ssock.close()
        
    finally:
        print("Closing socket")
        ssock.close()
      


if __name__ == "__main__":
    main()
