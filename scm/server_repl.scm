;;;; server_repl.scm
;;;; By: asbffgc
;;;; Date: 16 December 2024
;;;; Simple placeholder Scheme script
;;;; that loads a server REPL.
(use-modules (system repl server))

;; Geiser only connects if server runs in same shell,
;; WSL server needs WSL Geiser, MINGW etc
(run-server (make-tcp-server-socket    ; Create a new REPL server on 127.0.0.1
             #:host "127.0.0.1"
             #:port 37146))

;(run-server (make-unix-domain-server-socket #:path "/tmp/melty"))
