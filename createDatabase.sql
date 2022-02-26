DROP TABLE Product, History;
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
    id_product int       NOT NULL,
    price      float     NOT NULL,
    status     tinyint(1) NOT NULL,
    timestamp  timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,

    FOREIGN KEY (id_product) REFERENCES Product (id)

);

INSERT INTO Product (id, titre, description, url)
VALUES (1, 'coucou', 'description courte wallah', 'www.coucou.fr');
INSERT INTO History (id_product, price, status)
VALUES (1, 123.32, 0);