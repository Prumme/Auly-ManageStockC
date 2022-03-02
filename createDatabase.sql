DROP database test;
create database test;
use test;
CREATE TABLE Product
(
    id          int PRIMARY KEY AUTO_INCREMENT,
    titre       varchar(255) NOT NULL,
    description varchar(255) NULL,
    url         varchar(255) NOT NULL

);

ALTER TABLE Product
    AUTO_INCREMENT = 1;

CREATE TABLE History
(
    id         int PRIMARY KEY AUTO_INCREMENT,
    id_product int        NOT NULL,
    price      float      NOT NULL,
    status     tinyint(1) NOT NULL,
    timestamp  timestamp  NOT NULL DEFAULT CURRENT_TIMESTAMP,

    FOREIGN KEY (id_product) REFERENCES Product (id) ON DELETE CASCADE

);

SET GLOBAL sql_mode=(SELECT REPLACE(@@sql_mode,'ONLY_FULL_GROUP_BY',''));

INSERT INTO test.Product (titre, description, url)
VALUES ('QL120 RGB - Noir', 'Ventilateur boitier - PWM - 525 à 1500 RPM - 26 dB - 41.8 CFM',
        'https://www.topachat.com/pages/detail2_cat_est_micro_puis_rubrique_est_w_tu_ven_puis_ref_est_in20000287.html');
INSERT INTO test.Product (titre, description, url)
VALUES ('CM SickleFlow 120 ARGB', 'Ventilateur boitier - PWM - 650 à 1800 RPM - 27 dB - 62 CFM',
        'https://www.topachat.com/pages/detail2_cat_est_micro_puis_rubrique_est_w_tu_ven_puis_ref_est_in11018740.html');

INSERT INTO test.Product (titre, description, url)
VALUES ('ll120 blanc', 'Vasy wesh laisse moi écrire ma description en paix gros fdp',
        'https://www.topachat.com/pages/detail2_cat_est_micro_puis_rubrique_est_w_tu_ven_puis_ref_est_in10110960.html');

INSERT INTO test.History (id_product, price, status, timestamp)
VALUES (1, 123.32, 0, '2022-02-20 03:31:30');
INSERT INTO test.History (id_product, price, status, timestamp)
VALUES (1, 949.99, 1, '2022-02-20 03:31:39');
INSERT INTO test.History (id_product, price, status, timestamp)
VALUES (1, 1899.99, 0, '2022-02-20 03:34:09');
INSERT INTO test.History (id_product, price, status, timestamp)
VALUES (1, 1899.99, 1, '2022-02-24 21:07:45');
INSERT INTO test.History (id_product, price, status, timestamp)
VALUES (1, 1899.99, 1, '2022-02-24 21:08:15');
INSERT INTO test.History (id_product, price, status, timestamp)
VALUES (1, 2889.99, 1, '2022-02-24 21:12:39');
INSERT INTO test.History (id_product, price, status, timestamp)
VALUES (1, 429.98, 0, '2022-02-24 21:13:48');
INSERT INTO test.History (id_product, price, status, timestamp)
VALUES (1, 429.98, 1, '2022-02-24 21:16:01');
INSERT INTO test.History (id_product, price, status, timestamp)
VALUES (1, 2889.99, 1, '2022-02-24 21:16:16');
INSERT INTO test.History (id_product, price, status, timestamp)
VALUES (1, 959.99, 0, '2022-02-24 21:16:46');
INSERT INTO test.History (id_product, price, status, timestamp)
VALUES (1, 959.99, 0, '2022-02-24 21:24:33');
INSERT INTO test.History (id_product, price, status, timestamp)
VALUES (1, 959.99, 0, '2022-02-24 21:24:47');
INSERT INTO test.History (id_product, price, status, timestamp)
VALUES (1, 959.99, 0, '2022-02-24 21:49:55');
INSERT INTO test.History (id_product, price, status, timestamp)
VALUES (1, 1899.99, 1, '2022-02-24 23:00:41');
INSERT INTO test.History (id_product, price, status, timestamp)
VALUES (1, 1899.99, 1, '2022-02-24 23:02:42');
INSERT INTO test.History (id_product, price, status, timestamp)
VALUES (1, 1899.99, 1, '2022-02-24 23:02:56');
INSERT INTO test.History (id_product, price, status, timestamp)
VALUES (1, 1899.99, 1, '2022-02-24 23:03:38');
INSERT INTO test.History (id_product, price, status, timestamp)
VALUES (2, 1899.99, 1, '2022-02-24 23:03:56');
INSERT INTO test.History (id_product, price, status, timestamp)
VALUES (2, 1899.99, 1, '2022-02-24 23:11:18');
INSERT INTO test.History (id_product, price, status, timestamp)
VALUES (2, 1899.99, 1, '2022-02-24 23:11:35');
INSERT INTO test.History (id_product, price, status, timestamp)
VALUES (1, 1234.95, 1, '2022-02-26 14:17:33');
INSERT INTO test.History (id_product, price, status, timestamp)
VALUES (3, 1234.95, 1, '2022-02-26 14:20:31');
INSERT INTO test.History (id_product, price, status, timestamp)
VALUES (3, 1234.95, 1, '2022-02-26 14:24:55');

# Requête pour la liste des objets et leurs status
/*SELECT Product.id, Product.titre, H.timestamp, H.price, H.status
FROM Product
         INNER JOIN History H ON Product.id = H.id_product
         INNER JOIN
     (SELECT id, id_product, price, status, MAX(timestamp) Maxdate FROM History GROUP BY id_product) q
     on q.id_product = H.id_product AND q.Maxdate = H.timestamp;*/


# SELECT Product.id, Product.titre, H.timestamp, H.price, H.status FROM Product INNER JOIN History H ON Product.id = H.id_product INNER JOIN ( SELECT id, id_product, price, status, MAX(timestamp) Maxdate FROM History GROUP BY id_product ) q on q.id_product = H.id_product AND q.Maxdate = H.timestamp;