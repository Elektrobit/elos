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
c_autodoc_roots = []

c_autodoc_compilation_args = [
#    '-DSPHINX_C_AUTODOC_USE_BROKEN_FUNC_POINTER_TYPEDEFS',
]
c_autodoc_ignore_statements = [
    '__BEGIN_DECLS',
    '__END_DECLS',
]

set_type_checking_flag = True

import os

def populate_c_autodoc_roots():
    global c_autodoc_roots
    source_directories = ['./src']
    excluded_directories = [
        './src/clients',
        './src/demos',
        './src/version',
        './src/libelos/private',
        './src/plugins/clients/dummy',
        './src/plugins/storagebackends/dummy',
        './src/plugins/scanners/dummy'
    ]
    excluded_directories = [os.path.abspath(directory) for directory in excluded_directories]
    for source_dir in source_directories:
        if os.path.exists(source_dir):
            for current_root, subdirectories, _ in os.walk(source_dir):
                normalized_root = os.path.abspath(current_root)
                if not any(normalized_root.startswith(excluded_dir) for excluded_dir in excluded_directories):
                    c_autodoc_roots.append(current_root)

def pre_process_C_files(app, filename, contents, *args):
    _, file_ext = os.path.splitext(filename)
    if file_ext == '.h':
        modified_contents = contents[0]
        for element in c_autodoc_ignore_statements:
            modified_contents = modified_contents.replace(element, '')
        contents[:] = [modified_contents]

def setup(sphinx):
    populate_c_autodoc_roots()
    sphinx.connect("c-autodoc-pre-process", pre_process_C_files)
