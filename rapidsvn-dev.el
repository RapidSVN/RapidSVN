;;;; Emacs Lisp help for writing Rapidsvn code. ;;;;

;;; In C++ files, put something like this to load this file automatically:
;;
;;   /* -----------------------------------------------------------------
;;    * local variables:
;;    * eval: (load-file "../rapidsvn-dev.el")
;;    * end:
;;    */
;;
;; (note: make sure to get the path right in the argument to load-file).



;;; Rapidsvn C++ conventions
(if (eq major-mode 'c++-mode)
    (progn
      (c-add-style "rapidsvn" '("bsd" (c-offsets-alist . ((inextern-lang . 0)))))
      (c-set-style "rapidsvn")
      (setq c-basic-offset 2)
      ))
(setq indent-tabs-mode nil)
(setq angry-mob-with-torches-and-pitchforks t)


(message "loaded rapidsvn-dev.el")
