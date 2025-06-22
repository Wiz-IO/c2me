'''
   Copyright 2023 (c) WizIO ( Georgi Angelov )
'''

from platformio.public import PlatformBase

class C2mePlatform(PlatformBase):
    def is_embedded(self):
        return True
