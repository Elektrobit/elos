// SPDX-License-Identifier: MIT

#pragma once

#define ELOS_SQL_CREATE_TABLE_EVENTS            \
    "CREATE TABLE IF NOT EXISTS events ("       \
    " id INTEGER PRIMARY KEY AUTOINCREMENT,"    \
    " message_code INTEGER NOT NULL,"           \
    " classification INTEGER NOT NULL,"         \
    " severity  INTEGER NOT NULL,"              \
    " date_s INTEGER DEFAULT(DATETIME('now'))," \
    " date_ns INTEGER DEFAULT(0),"              \
    " payload TEXT,"                            \
    " pid INTEGER DEFAULT(0),"                  \
    " app_name VARCHAR(1024),"                  \
    " file_name VARCHAR(1024)"                  \
    ");"

#define ELOS_SQL_INSERT_EVENT                                                                        \
    "INSERT INTO events"                                                                             \
    " (message_code, classification, severity, date_s, date_ns, payload, pid, app_name, file_name) " \
    "VALUES"                                                                                         \
    " (?, ?, ?, ?, ?, ?, ?, ?, ?);"
