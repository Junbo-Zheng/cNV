project('cNV-meson', 'c')

incdir = include_directories('./cJSON', './nv')

executable('cNV-meson',
  sources: ['nv/nv.c', 'cJSON/cJSON.c', 'test.c','cJSON/cJSON_Utils.c'],
  c_args: ['-Wall', '-Wextra', '-g', '-DCONFIG_NV_DEBUG_MOCK_DATA=1', '-DCONFIG_NV_DEBUG_LOG=1'],
  include_directories : incdir
)
