import glob
import os 

images = glob.glob('*.jpg')
vector_text = "10000000"
id = 0
for image in images:
    filename = os.path.basename(image)
    #index_of_dot = filename.index('.')
    #file_name_without_extension = filename[:index_of_dot]
    os.rename(filename,r'3-'+str(id)+'-JN JS.jpg')
    id += 1
    #f= open(file_name_without_extension+".txt","w+")
    #f.write(vector_text)
    #f.close() 

