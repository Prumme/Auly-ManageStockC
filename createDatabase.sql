DROP database test;
create database test;
use test;
SET GLOBAL sql_mode=(SELECT REPLACE(@@sql_mode,'ONLY_FULL_GROUP_BY',''));
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