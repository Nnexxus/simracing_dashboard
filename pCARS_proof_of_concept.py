# -*- coding: utf-8 -*-
"""
Created on Wed Jul 31 16:46:20 2013

@author: Nnexxus
"""

#!/usr/bin/python
# -*- coding: utf-8 -*-

import Tkinter 
import mmap
import struct
import re
import serial
import time

UPDATE_FREQUENCY = 100 # ms

class PCARSplugin:
    def __init__(self):
        self._process_header_file()
        self.shmem = mmap.mmap(0, self.__struct.size, "$pcars$", mmap.ACCESS_READ)
        self.update()

    def _process_header_file(self):
        """Parse the C++ header file to retrieve the field names and types"""
        with open("SharedMemory.h") as f:
            line = f.readline()
            while not "typedef struct" in line:
                line = f.readline()
            print "Analyzing SharedMemory structure"
            variables_and_types = list()
            while not "} SharedMemory;" in line:
                line = f.readline()
                fields = line.split()
                if len(fields) >= 2:
                    if re.match(r"m\w*\[STRING_LENGTH_MAX\];", fields[1]):
                        variable = fields[1].split('[')[0]+";"
                        ctype = "string"
                        # print 1, variable
                        variables_and_types.append((variable,ctype))
                    elif re.match("^m\w*;$", fields[1]):
                        variable = fields[1]
                        ctype = fields[0]
                        # print 2, variable
                        variables_and_types.append((variable,ctype))
                    elif len(fields) >= 3 and re.match("^m\w*;$", fields[2]):
                        variable = fields[2]
                        ctype = "%s %s" % (fields[0], fields[1])
                        # print 3, variable
                        variables_and_types.append((variable,ctype))
            print "End of the SharedMemory structure"
            
            self._struct_string = ""
            self.variables = list()
            self._values = dict()
            self._types = dict()
            for (variable, ctype) in variables_and_types:
                self.variables.append(variable)
                self._values[variable] = "uninitialized"
                self._types[variable] = ctype
                if ctype == "int":
                    self._struct_string += "i"
                elif ctype == "unsigned int":
                    self._struct_string += "I"
                elif ctype == "float":
                    self._struct_string += "f"
                elif ctype == "string":
                    self._struct_string += "256s"
                else:
                    UserWarning('Unknown C type in SharedMemory structure')
            self.__struct = struct.Struct(self._struct_string)
            print "Shared Memory structure size : %d" % self.__struct.size
        
    def update(self):
        values = self.__struct.unpack_from(self.shmem)
        for (variable, value) in zip(self.variables, values):
            self._values[variable] = value

class DebugGUI(Tkinter.Label):
    def __init__(self,root,plugin,serial=None):
        Tkinter.Label.__init__(self,root,text='Starting...', justify=Tkinter.LEFT)
        self.plugin = plugin
        self.serial = serial
        self.font=('Helvetica', 12, 'normal')
        self.__setitem__('font',self.font)
        self.after(UPDATE_FREQUENCY,self.loop)
    
    def loop(self):
        # print "update"
        self.plugin.update()
        self.display()
        if self.serial is not None:
            self.send_serial()
        self.after(UPDATE_FREQUENCY,self.loop)
    
    def display(self):
        # print "display"
        string = ""
        for variable in self.plugin.variables:
            if self.plugin._types[variable] == "string":
                string_value = self.plugin._values[variable]
                string += "%s %s\n" % (variable, string_value[0:string_value.index("\0")])            
            else:
                string += "%s %s\n" % (variable, self.plugin._values[variable])
        self.__setitem__('text',string)
        
    def send_serial(self):
        # print "serial"
        # \n\r
        self.serial.write("%3d %1d\0" % (self.plugin._values["mSpeed;"], self.plugin._values["mGear;"]))
        #self.serial.write("test\n")
        
if __name__=="__main__":
    
    plugin = PCARSplugin()    
    
    with serial.Serial('COM5', 115200) as arduino:
        time.sleep(2) # To give the Arduino some time to initialize the serial link
        # arduino = None
        arduino.write("START\0")        
        
        root = Tkinter.Tk() 
        root.title('Project CARS TM extractor (in Python !)')
        label = DebugGUI(root,plugin,arduino)
        label.pack()
        root.mainloop()