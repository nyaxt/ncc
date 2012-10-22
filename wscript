VERSION='0.0.1'
APPNAME='ncc'

top = '.'
out = 'build'

def options(opt):
	opt.load('compiler_cxx')

def configure(conf):
	conf.load('compiler_cxx')

	conf.env.append_unique('CXXFLAGS', ['-Wall', '-g', '-march=native', '-std=c++0x', '-fno-operator-names'])
	conf.env.append_unique('INCLUDES', ['/home/kouhei/local/include', '/usr/local/include', '/opt/local/include'])
	conf.env.append_unique('LIBPATH', ['/home/kouhei/local/lib', '/usr/local/lib', '/opt/local/lib'])
	conf.env.append_unique('CXXFLAGS', '-O0')

	conf.find_program('leg')

	conf.check_cxx(uselib_store='XBYAK', header_name="xbyak/xbyak.h")

	conf.load('compiler_cxx')

def build(bld):
	bld(rule='leg ${SRC} -o ${TGT}', source='lang.leg', target='parse.impl.h')

	bld.stlib(
		target = 'nccl',
		source = '''
		ast.cpp
		parser.cpp
		''',
		)

	bld.program(target = 'ncc', use = 'nccl', source = 'main.cpp')

