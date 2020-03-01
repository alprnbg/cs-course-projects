#lang scheme

(define FARMS '(
                (farmA 100 (apricot apple blueberry))
                (farmB 90 (broccoli carrot grape))
                (farmC 75 (corn grape lemon))
                (farmD 75 ())))

(define CUSTOMERS '(
                    (john (farmA farmC) (apricot lemon))
                    (james (farmB farmC) (grape corn))
                    (arya (farmB farmD) (grape broccoli))
                    (elenor () ())))
(define CROPS '(
                (apricot farmA 10)
                (apple farmA 12)
                (blueberry farmA 15)
                (broccoli farmB 8)
                (carrot farmB 5)
                (grape farmB 10)
                (corn farmC 9)
                (grape farmC 12)
                (lemon farmC 10)))


(define (get-index mylist index)
  (cond
    ((equal? 0 index) (car mylist))
    (else
     (if (or (equal? (length mylist) 1) (equal? (length mylist) 0)) null (get-index (cdr mylist) (- index 1))))))


(define (get-name mylist name index)
  (cond
    ((equal? (get-index (car mylist) index) name) (car mylist))
    (else
     (if (or (equal? (length mylist) 1) (equal? (length mylist) 0)) null (get-name (cdr mylist) name index)))))


(define (TRANSPORTATION-COST farm-name)
  (define result (get-index (get-name FARMS farm-name 0) 1))
  (if (null? result) 0 result))


(define (AVAILABLE-CROPS farm-name)
  (define result (get-index (get-name FARMS farm-name 0) 2))
  (if (null? result) null result))


(define (INTERESTED-CROPS customer-name)
  (define result (get-index (get-name CUSTOMERS customer-name 0) 2))
  (if (null? result) null result))


(define (CONTRACT-FARMS customer-name)
  (define result (get-index (get-name CUSTOMERS customer-name 0) 1))
  (if (null? result) null result))


(define (CONTRACT-WITH-FARM farm-name)
  (CONTRACT-WITH-FARM-2 farm-name CUSTOMERS null))


(define (CONTRACT-WITH-FARM-2 farm-name customers result)
  (if (null? customers) result
       (if (member farm-name (get-index (car customers) 1))
           (CONTRACT-WITH-FARM-2 farm-name (cdr customers) (append result (list (get-index (car customers) 0))))
           (CONTRACT-WITH-FARM-2 farm-name (cdr customers) result))))


(define (INTERESTED-IN-CROP crop-name)
  (INTERESTED-IN-CROP-2 crop-name CUSTOMERS null))


(define (INTERESTED-IN-CROP-2 crop-name customers result)
  (if (null? customers) result
       (if (member crop-name (get-index (car customers) 2))
           (INTERESTED-IN-CROP-2 crop-name (cdr customers) (append result (list (get-index (car customers) 0))))
           (INTERESTED-IN-CROP-2 crop-name (cdr customers) result))))


(define (MIN-SALE-PRICE crop-name)
  (if (not (null? (get-name CROPS crop-name 0)))
      (MIN-SALE-PRICE-2 crop-name CROPS (get-index (get-name CROPS crop-name 0) 2)) 0))


(define (MIN-SALE-PRICE-2 crop-name crops min)
  (if (null? crops) min
      (if (and (< (get-index (car crops) 2) min) (equal? crop-name (get-index (car crops) 0)))
          (MIN-SALE-PRICE-2 crop-name (cdr crops) (get-index (car crops) 2))
          (MIN-SALE-PRICE-2 crop-name (cdr crops) min))))


(define (CROPS-BETWEEN min max)
  (CROPS-BETWEEN-2 CROPS min max null))


(define (CROPS-BETWEEN-2 crops min max result)
  (if (null? crops) result
      (if (and (<= (get-index (car crops) 2) max) (<= min (get-index (car crops) 2)))
          (CROPS-BETWEEN-2 (cdr crops) min max (append result (list (get-index (car crops) 0))))
          (CROPS-BETWEEN-2 (cdr crops) min max result))))


(define (return result)
  (if (equal? result +inf.0) 0 result))


(define (find-crop-cost crops farm-name crop-name)
  (if (null? crops) +inf.0
      (if (and (equal? (get-index (car crops) 0) crop-name) (equal? (get-index (car crops) 1) farm-name))
          (get-index (car crops) 2)
          (find-crop-cost (cdr crops) farm-name crop-name))))


(define (calculate-cost-farm-crop farm-name crop-name)
  (if (member crop-name (get-index (get-name FARMS farm-name 0) 2))
      (+ (get-index (get-name FARMS farm-name 0) 1) (find-crop-cost CROPS farm-name crop-name))
      +inf.0))


(define (BUY-PRICE customer-name crop-name)
  (BUY-PRICE-2 (get-index (get-name CUSTOMERS customer-name 0) 1) crop-name +inf.0))


(define (BUY-PRICE-2 contracted-farms crop-name result)
  (if (null? contracted-farms) (return result)
   (if (< (calculate-cost-farm-crop  (car contracted-farms) crop-name) result)
       (BUY-PRICE-2 (cdr contracted-farms) crop-name (calculate-cost-farm-crop (car contracted-farms) crop-name))
       (BUY-PRICE-2 (cdr contracted-farms) crop-name result))))


(define (TOTAL-PRICE customer-name)
  (TOTAL-PRICE-2 (get-index (get-name CUSTOMERS customer-name 0) 2) customer-name 0))


(define (TOTAL-PRICE-2 crops customer-name result)
  (if (null? crops) result
      (TOTAL-PRICE-2 (cdr crops) customer-name (+ result (BUY-PRICE customer-name (car crops))))))
