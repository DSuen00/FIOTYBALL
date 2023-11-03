def write_motor_data(file_name, motor_data):
    file = open(file_name,"w")
    file.write(str(motor_data))
    file.close()
    return

def read_motor_data(file_name):
    file = open(file_name)
    data = str(file.read())
    file.close()
    return data
