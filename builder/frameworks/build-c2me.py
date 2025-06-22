'''
   Copyright 2025 (c) WizIO ( Georgi Angelov )
'''

from os.path import join, exists, basename
from SCons.Script import DefaultEnvironment, Builder

env = DefaultEnvironment()

core = ["-march=armv7-m", "-mthumb", "-mfloat-abi=soft", "-fpic"]

env.Append(
   CPPDEFINES  = [  ],
   CPPPATH = [
      join("$PROJECT_DIR", "src"),
      join("$PROJECT_DIR", "lib"),
      join("$PROJECT_DIR", "include"),
   ],
   CFLAGS = [
      core,
      "-Os",
      "-std=gnu99",
      "-ffunction-sections",
      "-fdata-sections",
      "-fomit-frame-pointer",
      "-fno-asm",
      "-fno-common",
      "-fno-builtin",
      "-fno-strict-aliasing",
      "-fsingle-precision-constant",
      "-Wall",
      "-Wno-unused-parameter",
      "-Wno-unused-function",
      "-Wno-unused-but-set-variable",
      "-Wno-unused-variable",
      "-Wno-unused-value",

   ],
   CCFLAGS = [  ],
   CXXFLAGS = [
      '-fno-rtti',
      '-fno-exceptions',
      '-fno-threadsafe-statics',
      '-fno-non-call-exceptions',
      '-fno-use-cxa-atexit',
   ],
   LINKFLAGS = [
      core,
      "-Os",
      "--entry=main",
      "-Wl,--gc-sections",
      "-lnosys",
      "-nostartfiles",
      "--specs=nano.specs",
      "--specs=nosys.specs",
      #"-u", "_printf_float",
   ],
   LIBS = [
      "api"
   ],
   LIBSOURCE_DIRS  = [
      join('$PROJECT_DIR', 'lib'),
   ],
   LIBPATH = [
      join('$PROJECT_DIR', 'lib'),
      join("$FRAMEWORK_DIR"),
   ],
   LDSCRIPT_PATH = join("$FRAMEWORK_DIR", "c.ld"),

   BUILDERS = dict(
      ELF2HEX = Builder(
         action = env.VerboseAction(' '.join([ '$OBJCOPY', '-O',  'ihex', '$SOURCES', '$TARGET', ]), 'Building HEX $TARGET'),
         suffix = '.hex'
      ),
      ELF2BIN = Builder(
         action = env.VerboseAction(' '.join([ '$OBJCOPY', '-O',  'binary', '-S', '$SOURCES', '$TARGET', ]), 'Building BIN $TARGET'),
         suffix = '.bin'
      ),
   ),
)
