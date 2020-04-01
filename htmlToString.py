
data = ''
with open('./include/index.html', 'r') as myfile:
    data = myfile.read().replace('\n', '')

with open('./include/html_index.h', 'w') as headerfile:
    headerfile.write(
        'const char INDEX_HTML[] PROGMEM = R\"=====('+data+')=====\";')
