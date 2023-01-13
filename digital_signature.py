import rsa 

def get_key(): 		#Key generation 
	(pubkey, privkey) = rsa.newkeys(1024) 	#Generate public-private key pair
	pub = pubkey.save_pkcs1() 
	pubfile = open('public.pem','wb') 
	pubfile.write(pub) 
	pubfile.close() 
 
	pri = privkey.save_pkcs1() 
	prifile = open('private.pem','wb') 
	prifile.write(pri) 
	prifile.close() 
 

def encry_sig(): # signature
	msg_file = open('message.txt') 
	message = msg_file.read().encode('utf-8') 
	with open('private.pem') as prifile: 
		p = prifile.read() 
		prikey = rsa.PrivateKey.load_pkcs1(p) 
	signature = rsa.sign(message,prikey,'SHA-1') 	# signature
	sig_file = open('sig_file.txt','wb') 	#write file
	sig_file.write(signature) 
	sig_file.close() 
	prifile.close() 

def decry_sig(): 	# verification
	sig_file = open('sig_file.txt','rb',) 
	#message = 'abc'.encode('utf-8') 
	sig_msg = sig_file.read() 
	msg_file = open('message.txt') 
	message = msg_file.read().encode('utf-8') 
	with open('public.pem') as pubfile: 
		p = pubfile.read() 
		pubkey = rsa.PublicKey.load_pkcs1(p) 
	sig_file.close() 
	try:	 # verification
		rsa.verify(message, sig_msg, pubkey) 	# Verify that the return value of this function is the hash method. If the verification fails, an error will be reported
		return True 
	except: 
		return False 


encry_sig() 

print(decry_sig())

