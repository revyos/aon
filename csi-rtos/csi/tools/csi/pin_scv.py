from .tools import *
from .ioctrl_attr import *
import json
import operator

class ScvPinIteam(object):
    def __init__(self, pin_info):
        self.pins_dic = []
        self.pin_num = pin_info.chip_pin
        self.pin_name = pin_info.name

        pin = {'pin_num': str(pin_info.chip_pin), 'pin_name': pin_info.name, \
                   'pin_select': pin_info.name + '_GPIO', 'pin_func': 'PIN_FUNC_GPIO'}
        self.pins_dic.append(pin)
        for func in pin_info.pin_func:
            pin_func = pin_info.name
            for k, v in func.__dict__.items():
                pin_func += '_' + k
            pin = {'pin_num': str(pin_info.chip_pin), 'pin_name': pin_info.name, \
                   'pin_select': pin_func, 'pin_func': pin_func}
            self.pins_dic.append(pin)

class CdkPinJson:
    def __init__(self, chip_info):
        chip_pins_info = IoctrlTcip(chip_info.devices, chip_info.device_attr)
        self.json_pins = []
        for pin in chip_pins_info.pins:
            self.json_pins += ScvPinIteam(pin).pins_dic

    def save(self, filename):
        dcit_sorted(self.json_pins, 'pin_num')
        json_str = json.dumps(dcit_sorted(self.json_pins, 'pin_num'), \
                              sort_keys=True, indent=4, separators=(',', ':'))
        write_file(json_str, filename)

class CdkPinSvc:
    def __init__(self, chip_info, xml_file):
        self.prj_xml = XmlTree(xml_file)
        self.Pins = self.prj_xml.find('Pins')
        self.PinCount = self.prj_xml.find('PinCount')
        self.ChipName = self.prj_xml.find('ChipName')
        self.ChipName.text = chip_info.vendor.chip
        self.chip_pins_info = IoctrlTcip(chip_info.devices, chip_info.device_attr)
        cmpfun = operator.attrgetter('chip_pin')
        self.chip_pins_info.pins.sort(key= cmpfun)
        pin_count = 1;
        for v in self.chip_pins_info.pins:
            while pin_count < v.chip_pin:
                self.Pins.append(self._def_pin(pin_count))
                pin_count += 1
            self.add_pin(v)
            pin_count += 1
        self.PinCount.text = str(pin_count - 1)

    def add_pin(self, pin_info):
        Pin = ET.Element('Pin')
        Pin.attrib = {'pinNum': str(pin_info.chip_pin), 'group': pin_info.name[0:2]}
        PinFunctions = ET.Element('PinFunctions')
        PinFunctions.attrib = {'select': pin_info.name + '_GPIO'}

        Func = ET.Element('Func')
        Func.attrib = {'deviceId':'CPIO' + pin_info.name[1], 'csi_pin_name':'PORT' + pin_info.name[1] + '_PIN' + pin_info.name[2:]}
        Func.text = pin_info.name + '_GPIO'
        PinFunctions.append(Func)

        for v in pin_info.pin_func:
            for kk, vv in v.__dict__.items():
                split = kk.split('_')
                dev_name = split[1]
                Func = ET.Element('Func')
                Func.attrib = {'deviceId': dev_name}
                Func.text = pin_info.name + '_' + kk
                PinFunctions.append(Func)
        Pin.append(PinFunctions)

        Direction = ET.Element('Direction')
        Direction.attrib = {'Comment':"", 'displayName':"Direction"}
        Direction.text = 'output'
        Pin.append(Direction)

        Pullup = ET.Element('Pullup')
        Pullup.attrib = {'Comment':"", 'displayName':"Pullup"}
        Pullup.text = 'pull none'
        Pin.append(Pullup)

        self.Pins.append(Pin)

    def _def_pin(self, pinnum):
        Pin = ET.Element('Pin')
        Pin.attrib =  {'pinNum':str(pinnum) ,'canModify': 'false'}
        PinFunctions = ET.Element('PinFunctions')
        PinFunctions.attrib = {'select': 'Fixed'}
        Func = ET.Element('Func')
        Func.text = ' '
        PinFunctions.append(Func)
        Pin.append(PinFunctions)
        return Pin

    def save(self, filename):
        self.prj_xml.write(filename)

