cmake_minimum_required(VERSION 3.15)
project(progetto_colonne)

set(CMAKE_CXX_STANDARD 11)

add_executable(progetto_colonne main.cpp Tabella.cpp Tabella.h Colonna.h ColonnaInt.cpp ColonnaInt.h
        ColonnaFloat.cpp ColonnaFloat.h ColonnaText.cpp ColonnaText.h data.h data.cpp ColonnaDate.cpp
        ColonnaDate.h Time.cpp Time.h ColonnaChar.cpp ColonnaChar.h ColonnaTime.cpp ColonnaTime.h
        CampoNonTrovato.h PrimaryKeyAlreadyExisting.h InvalidOperator.h SecKeyNotFound.h SecKeyError.h
        PrimKeyError.h NotNullError.h InvalidCondition.h FileError.h SalvataggioCaricamento.hpp ControlloSintassi.cpp
        ControlloSintassi.h FormatTypeError.h SecKeyAlreadyExisting.h Comandi.hpp InexistentTable.h
        ValueNotFound.h InsertAutoIncrement.h LinkedError.h TableAlreadyExisting.h InvalidMatch.h InvalidDate.h InvalidTime.h)