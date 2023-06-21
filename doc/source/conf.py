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
    'sphinx.ext.autodoc',
    'sphinx.ext.doctest',
    'sphinxcontrib.programoutput',
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

templates_path = ['_templates']
exclude_patterns = []

language = 'en'

# -- Options for HTML output -------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#options-for-html-output

html_theme = 'alabaster'
html_static_path = ['_static']
html_logo = './_static/elos_blue.png'

favicons = [
    "favicon_16x16.png",
    "favicon_32x32.png",
]

# c-autodoc
c_autodoc_roots = [
    '../../src/',
    '../../src/libelos/public/elos',
    '../../src/processfilter/public',
    '../../src/client_manager/public',
    '../../src/config/public',
    '../../src/event/public',
    '../../src/eventfilter/public',
    '../../src/eventlogging/public',
    '../../src/eventprocessor/public',
    '../../src/libelos/public',
    '../../src/messages/public',
    '../../src/plugin/public',
    '../../src/pluginmanager/public',
    '../../src/rpnfilter/public',
    '../../src/scanner_manager/public',
    '../../src/eventbuffer/public',
    '../../src/scanner/public',
]

set_type_checking_flag = True
