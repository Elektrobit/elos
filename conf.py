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

templates_path = ['doc/_templates']
exclude_patterns = ['build/*/cmake/_deps/*', 'README.md', '.venv']
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
    './src/components/connectionmanager/interface',
    './src/components/connectionmanager/private',
    './src/components/connectionmanager/public',
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
    './src/components/messages/interface',
    './src/components/messages/private',
    './src/components/messages/public',
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
    './src/components/scanner/interface',
    './src/components/scanner/private',
    './src/components/scanner/public',
    './src/components/scanner_manager/interface',
    './src/components/scanner_manager/private',
    './src/components/scanner_manager/public',
    './src/components/storagemanager/interface',
    './src/components/storagemanager/private',
    './src/components/storagemanager/public',
    './src/libelos/public',
    './src/libelos/public/elos',
    './src/libelosplugin/public',
    './src/libelosplugin/private',
    './src/libelosplugin/interface',
    './src/plugins/scanners/kmsg/interface/',
    './src/plugins/scanners/kmsg/private/',
    './src/plugins/scanners/syslog/interface/',
    './src/plugins/scanners/syslog/private/',
    './src/plugins/scanners/shmem/interface/',
    './src/plugins/scanners/shmem/private/',
    './src/plugins/storagebackends/jsonbackend/interface/',
    './src/plugins/storagebackends/jsonbackend/private/',
    './src/plugins/storagebackends/nosqlbackend/interface/',
    './src/plugins/storagebackends/nosqlbackend/private/',
    './src/plugins/storagebackends/sqlbackend/interface/',
    './src/plugins/storagebackends/sqlbackend/private/',
    './src/plugins/storagebackends/influxdbbackend/interface/',
    './src/plugins/storagebackends/influxdbbackend/private/',
]

set_type_checking_flag = True
