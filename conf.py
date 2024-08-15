# Configuration file for the Sphinx documentation builder.
#
# For the full list of built-in configuration values, see the documentation:
# https://www.sphinx-doc.org/en/master/usage/configuration.html

# -- Project information -----------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#project-information

project = 'elos'
copyright = '2023, wolfgang.gehrhardt@emlix.com'
author = 'wolfgang.gehrhardt@emlix.com'

# -- General configuration ---------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#general-configuration

extensions = [
    'myst_parser',
    'sphinx.ext.autodoc',
    'sphinx.ext.doctest',
    'sphinxcontrib.programoutput',
    'sphinxcontrib.plantuml',
    'sphinx_favicon',

    "sphinx.ext.autosectionlabel",  # better cross referencing

    # copy button on code blocks in HTML doc
    'sphinx_copybutton',

    # for code documentation & referencing
    'sphinx.ext.viewcode',
    'sphinx.ext.napoleon',
    'sphinx_c_autodoc',
    'sphinx_c_autodoc.napoleon',
    'sphinx_c_autodoc.viewcode',
]

myst_enable_extensions = ["tasklist"]
autosectionlabel_prefix_document = True
autosectionlabel_maxdepth = 2

templates_path = ['doc/_templates']
exclude_patterns = ['build/deps/**', 'build/*/cmake/_deps/*', 'README.md', '.venv']
source_suffix = {
    '.rst': 'restructuredtext',
    '.md': 'markdown',
}

language = 'en'

# -- Options for HTML output -------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#options-for-html-output

html_theme = 'alabaster'
html_static_path = ['doc/static']
html_logo = 'doc/static/elos_blue.png'

favicons = [
    "favicon_16x16.png",
    "favicon_32x32.png",
]

# plantuml
plantuml_output_format = 'png'
plantuml_batch_size = 100
plantuml_cache_path = 'build/_plantum'

# c-autodoc
c_autodoc_roots = [
    './src/',
    './src/common/interface',
    './src/components/clientmanager/interface',
    './src/components/clientmanager/private',
    './src/components/clientmanager/public',
    './src/components/config/interface',
    './src/components/config/private',
    './src/components/config/public',
    './src/components/eloslog/interface',
    './src/components/eloslog/private',
    './src/components/eloslog/public',
    './src/components/event/interface',
    './src/components/event/private',
    './src/components/event/public',
    './src/components/eventbuffer/interface',
    './src/components/eventbuffer/private',
    './src/components/eventbuffer/public',
    './src/components/eventdispatcher/interface',
    './src/components/eventdispatcher/private',
    './src/components/eventdispatcher/public',
    './src/components/eventfilter/interface',
    './src/components/eventfilter/private',
    './src/components/eventfilter/public',
    './src/components/eventlogging/interface',
    './src/components/eventlogging/private',
    './src/components/eventlogging/public',
    './src/components/eventprocessor/interface',
    './src/components/eventprocessor/private',
    './src/components/eventprocessor/public',
    './src/components/logger/interface',
    './src/components/logger/private',
    './src/components/logger/public',
    './src/components/plugincontrol/interface',
    './src/components/plugincontrol/private',
    './src/components/plugincontrol/public',
    './src/components/pluginmanager/interface',
    './src/components/pluginmanager/private',
    './src/components/pluginmanager/public',
    './src/components/processfilter/interface',
    './src/components/processfilter/private',
    './src/components/processfilter/public',
    './src/components/rpnfilter/interface',
    './src/components/rpnfilter/private',
    './src/components/rpnfilter/public',
    './src/components/scannermanager/interface',
    './src/components/scannermanager/private',
    './src/components/scannermanager/public',
    './src/components/storagemanager/interface',
    './src/components/storagemanager/private',
    './src/components/storagemanager/public',
    './src/libelos/public',
    './src/libelos/public/elos',
    './src/libelosdlt/interface',
    './src/libelosdlt/private',
    './src/libelosdlt/public',
    './src/libelosplugin/interface',
    './src/libelosplugin/private',
    './src/libelosplugin/public',
    './src/plugins/clients/tcp/interface/',
    './src/plugins/clients/tcp/interface/connectionmanager',
    './src/plugins/clients/tcp/interface/messagehandler',
    './src/plugins/clients/tcp/private/',
    './src/plugins/clients/tcp/private/connectionmanager',
    './src/plugins/clients/tcp/private/messagehandler',
    './src/plugins/clients/tcp/public/',
    './src/plugins/clients/tcp/public/connectionmanager',
    './src/plugins/clients/tcp/public/messagehandler',
    './src/plugins/scanners/oomkiller/interface/',
    './src/plugins/scanners/oomkiller/private/',
    './src/plugins/scanners/kmsg/interface/',
    './src/plugins/scanners/kmsg/private/',
    './src/plugins/scanners/shmem/interface/',
    './src/plugins/scanners/shmem/private/',
    './src/plugins/scanners/syslog/interface/',
    './src/plugins/scanners/syslog/private/',
    './src/plugins/storagebackends/dlt/interface/',
    './src/plugins/storagebackends/dlt/private/',
    './src/plugins/storagebackends/influxdbbackend/interface/',
    './src/plugins/storagebackends/influxdbbackend/private/',
    './src/plugins/storagebackends/jsonbackend/interface/',
    './src/plugins/storagebackends/jsonbackend/private/',
    './src/plugins/storagebackends/nosqlbackend/interface/',
    './src/plugins/storagebackends/nosqlbackend/private/',
    './src/plugins/storagebackends/sqlbackend/interface/',
    './src/plugins/storagebackends/sqlbackend/private/',
]
c_autodoc_compilation_args = [
#    '-DSPHINX_C_AUTODOC_USE_BROKEN_FUNC_POINTER_TYPEDEFS',
]
c_autodoc_ignore_statements = [
    '__BEGIN_DECLS',
    '__END_DECLS',
]

set_type_checking_flag = True

import os

def pre_process_C_files(app, filename, contents, *args):
    _, file_ext = os.path.splitext(filename)
    if file_ext == '.h':
        modified_contents = contents[0]
        for element in c_autodoc_ignore_statements:
            modified_contents = modified_contents.replace(element, '')
        contents[:] = [modified_contents]

def setup(sphinx):
    sphinx.connect("c-autodoc-pre-process", pre_process_C_files)
