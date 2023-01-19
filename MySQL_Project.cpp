#define _CRT_SECURE_NO_WARNINGS
#define NOMINMAX
#include <iostream>
#include <ctime>
#include <iomanip>
#include <random>
#include <vector>
#include <string>
#include <tabulate/tabulate.hpp>
#include <tabulate/table.hpp>
#include "mysql_connection.h"
#include <cppconn/driver.h>
#include <cppconn/statement.h>
#include <cppconn/resultset.h>
#include <cppconn/exception.h>
#include <cppconn/prepared_statement.h>

constexpr auto MONTANT_MINIMUM_TRANSFERT = 50000;
constexpr auto IDMIN = 100000000000;
constexpr auto IDMAX = 999999999999;

// D�claration des variables globales et structures
time_t now = time(nullptr);
tm temps = *localtime(&now);
short day = temps.tm_mday;
short month = temps.tm_mon + 1;
short year = temps.tm_year + 1900;

sql::Driver* driver;
sql::Connection* connection;
sql::Statement* stmt;
sql::PreparedStatement* pstmt = nullptr;
sql::ResultSet* resultat = nullptr;

// G�n�rer un code avec l'algorithme de Mersenne Twister
std::mt19937 generateCode;

struct Date {
    short jour = 0;
    short mois = 0;
    short annee = 0;
};

struct Adresse {
    std::string quartier{ "" };
    std::string ville{ "" };
};

struct Transfert {
    long long code = 0;
    std::string nomExpe{ "" };
    Adresse adresseExpe;
    std::string nomBene{ "" };
    Adresse adresseBene;
    long long numCarteIdBene = 0;
    Date dateExpe;
    Date dateAnnu;
    Date dateRetrait;
    long montantDepose = 0;
    bool retirer = false;
    bool annuler = false;
};

struct StringTransfert {
    std::string nomExpe{ "" };
    std::string adExpe{ "" };
    std::string nomBene{ "" };
    std::string adBene{ "" };
    std::string carteIdBene{ "" };
    std::string montant{ "" };
    std::string dateExpe{ "" };
    std::string dateAnnu{ "" };
    std::string dateRetrait{ "" };
};

enum color {
    noir,
    bleu,
    vert,
    bleuCiel,
    rouge,
    violet,
    jaune,
    blanc,
    blancSombre
};

// D�claration des fonctions
void do_transfer();
void connect_db();
bool find_transfer(const long long&, bool);
void remove_transfer();
void cancel_transfer();
void delete_transfer();
void alter_transfer();
void transfer_situation();
void list_all_transfer_onDate();
void list_all_transfer_betweenTwoDates();
void list_all_transfer_removedOnDate();
void list_all_transfer_removed_betweenTwoDates();
void list_all_transfer();
void list_all_transfer_removed();
void list_all_transfer_cancelled();

void animation(const std::string&);
void animationSansRetour(const std::string&);
void changerCouleur(WORD numeroCouleur);
void affichageErreur(const std::string&, color);
void exitProgram();
void clear();

// Fonctions servant d'entr�e securis�e
void secureEntry(std::string&);
void secureEntry(int&);
void secureEntry(long&);
void secureEntry(long long&);
void secureEntry(double&);

int main() {
    system("title FlashTrans");
    tabulate::Table menuUti;
    int option = 0;

    menuUti.add_row({ "MENU" });
    menuUti.add_row({ "[1]  ENREGISTRER UN TRANSFERT " });
    menuUti.add_row({ "[2]  RECHERCHER UN TRANSFERT " });
    menuUti.add_row({ "[3]  RETIRER UN TRANSFERT " });
    menuUti.add_row({ "[4]  ANNULER UN TRANSFERT " });
    menuUti.add_row({ "[5]  SUPPRIMER UN TRANSFERT " });
    menuUti.add_row({ "[6]  MODIFIER UN TRANSFERT " });
    menuUti.add_row({ "[7]  SITUATION DES TRANSFERTS (nombre de depots, montant total des depots) " });
    menuUti.add_row({ "[8]  LISTE DES TRANSFERTS " });
    menuUti.add_row({ "[9]  LISTE DES TRANSFERTS RETIRES " });
    menuUti.add_row({ "[10] LISTE DES TRANSFERTS ANNULES " });
    menuUti.add_row({ "[11] LISTE DES TRANSFERTS EFFECTUES A UNE DATE " });
    menuUti.add_row({ "[12] LISTE DES TRANSFERTS EFFECTUES ENTRE DEUX DATES " });
    menuUti.add_row({ "[13] LISTE DES TRANSFERTS RETIRES A UNE DATE " });
    menuUti.add_row({ "[14] LISTE DES TRANSFERTS RETIRES ENTRE DEUX DATES " });
    menuUti.add_row({ "[15] LISTE DES TRANSFERTS ANNULES A UNE DATE " });
    menuUti.add_row({ "[16] LISTE DES TRANSFERTS ANNULES ENTRE DEUX DATES " });
    menuUti.add_row({ "[0]  QUITTER L'APPLICATION" });

    menuUti.format()
        .width(145)
        .font_color(tabulate::Color::blue);

    menuUti[0].format()
        .padding_top(1)
        .padding_bottom(1)
        .font_style({ tabulate::FontStyle::underline })
        .font_background_color(tabulate::Color::white)
        .font_align(tabulate::FontAlign::center)
        .font_color(tabulate::Color::green);

    do {
        system("cls");
        std::cout << menuUti << std::endl;
        animationSansRetour("### Option >> ");
        secureEntry(option);
        switch (option) {
        case 1:
            system("cls");
            do_transfer();
            break;
        case 2:
            system("cls");
            long long code;
            animationSansRetour("### Entrez le code du transfert >>> ");
            secureEntry(code);
            if (!find_transfer(code, true)) {
                affichageErreur("### CE CODE NE FIGURE PAS DANS LA BASE DE DONNEES ###", rouge);
            }
            break;
        case 3:
            system("cls");
            remove_transfer();
            break;
        case 4:
            system("cls");
            cancel_transfer();
            break;
        case 5:
            system("cls");
            delete_transfer();
            break;
        case 6:
            system("cls");
            alter_transfer();
            break;
        case 7:
            system("cls");
            transfer_situation();
            break;
        case 8:
            system("cls");
            list_all_transfer();
            break;
        case 9:
            system("cls");
            list_all_transfer_removed();
            break;
        case 10:
            system("cls");
            list_all_transfer_cancelled();
            break;
        case 11:
            system("cls");
            list_all_transfer_onDate();
            break;
        case 12:
            system("cls");
            list_all_transfer_betweenTwoDates();
            break;
        case 13:
            system("cls");
            list_all_transfer_removedOnDate();
            break;
        case 14:
            system("cls");
            list_all_transfer_removed_betweenTwoDates();
            break;
        case 15:
            system("cls");
            break;
        case 16:
            system("cls");
            break;
        case 0:
            system("cls");
            exitProgram();
            break;
        default:
            affichageErreur("### ENTREZ UN CHOIX DISPONIBLE ###", rouge);
        }
        
        system("pause");
    } while (option != 0);

    return 0;
}

void connect_db() {
    try{
        driver = get_driver_instance();
        connection = driver->connect("tcp://127.0.0.1:3306", "root", "");
        connection->setSchema("transfert_db");
    }
    catch (sql::SQLException& e){
        std::cout << "# ERR: " << e.what();
        std::cout << " (code erreur MySQL: " << e.getErrorCode();
        std::cout << ", EtatSQL: " << e.getSQLState() << ")" << std::endl;
    }
}

void do_transfer() {
    Transfert transf;
    tabulate::Table enregistrement;
    enregistrement.add_row({ "EFFECTUER UN TRANSFERT" });
    enregistrement.format()
        .padding_bottom(1)
        .padding_top(1)
        .font_background_color(tabulate::Color::white)
        .font_align(tabulate::FontAlign::center)
        .width(145)
        .font_color(tabulate::Color::green)
        .font_style({ tabulate::FontStyle::underline });
    std::cout << enregistrement << std::endl;

    try {
        existe:
        transf.code = generateCode();
        if (find_transfer(transf.code, false)) goto existe;
    }
    catch (sql::SQLException& e) {
        std::cout << "# ERR: " << e.what();
        std::cout << " (code erreur MySQL: " << e.getErrorCode();
        std::cout << ", 2EtatSQL: " << e.getSQLState() << ")" << std::endl;
    }

    changerCouleur(bleu);
    animationSansRetour("### Code du Transfert -----> ");
    std::cout << transf.code << std::endl;
    changerCouleur(blanc);
    animationSansRetour("\n### Entrez le nom de l'exp�diteur >> ");
    secureEntry(transf.nomExpe);
    animationSansRetour("\n### Entrez la ville de l'exp�diteur >> ");
    secureEntry(transf.adresseExpe.ville);
    animationSansRetour("\n### Entrez le quartier de l'exp�diteur >> ");
    secureEntry(transf.adresseExpe.quartier);
    animationSansRetour("\n### Entrez le nom du b�n�ficiaire >> ");
    secureEntry(transf.nomBene);
    animationSansRetour("\n### Entrez le ville du b�n�ficiaire >>> ");
    secureEntry(transf.adresseBene.ville);
    animationSansRetour("\n### Entrez le quartier du b�n�ficiaire >>> ");
    secureEntry(transf.adresseBene.quartier);
    animationSansRetour("\n### Entrez le num�ro de carte id du b�n�ficiaire >>> ");
    secureEntry(transf.numCarteIdBene);
    while ((transf.numCarteIdBene < IDMIN) || (transf.numCarteIdBene > IDMAX)) {
        changerCouleur(rouge);
        animationSansRetour("### Le numero id doit faire 12 chiffres, Recommencez >>> ");
        changerCouleur(blanc);
        secureEntry(transf.numCarteIdBene);
    }

    animationSansRetour("\n### Entrez le montant � transf�rer >>> ");
    secureEntry(transf.montantDepose);
    while (transf.montantDepose < MONTANT_MINIMUM_TRANSFERT) {
        changerCouleur(rouge);
        animationSansRetour("### Le montant minimum a transferer c'est 50.000, Recommencez >>> ");
        changerCouleur(blanc);
        secureEntry(transf.montantDepose);
    }

    std::string date;
    std::string jour = std::to_string(day);
    std::string mois = std::to_string(month);
    std::string annee = std::to_string(year);

    if (day < 10) jour = "0" + jour;
    if (month < 10) mois = "0" + mois;

    date += jour;
    date += "/";
    date += mois;
    date += "/";
    date += annee;

    // Enregistrement du transfert dans la base de donn�es
    try {
        connect_db();
        std::string adresseExpediteur = transf.adresseExpe.ville + "/" + transf.adresseExpe.quartier;
        std::string adressebeneficiaire = transf.adresseBene.ville + "/" + transf.adresseBene.quartier;
        pstmt = connection->prepareStatement("INSERT INTO transfert_tb(code, nomExpe, adresseExpe, nomBene, adresseBene, carteIdBene, dateExpe, montant) VALUES(?, ?, ?, ?, ?, ?, ?, ?)");
        pstmt->setBigInt(1, std::to_string(transf.code));
        pstmt->setString(2, transf.nomExpe);
        pstmt->setString(3, adresseExpediteur);
        pstmt->setString(4, transf.nomBene);
        pstmt->setString(5, adressebeneficiaire);
        pstmt->setBigInt(6, std::to_string(transf.numCarteIdBene));
        pstmt->setString(7, date);
        pstmt->setString(8, std::to_string(transf.montantDepose));
        pstmt->execute();
        affichageErreur("### TRANSFERT EFFECTUE AVEC SUCCES ###", vert);

        delete connection;
        delete pstmt;
    }
    catch (sql::SQLException& e) {
        std::cout << "# ERR: " << e.what();
        std::cout << " (code erreur MySQL: " << e.getErrorCode();
        std::cout << ", EtatSQL: " << e.getSQLState() << ")" << std::endl;
    }
}

bool find_transfer(const long long& codeTransf, bool afficher = false) {
    int trouve = 0;

    try {
        connect_db();
        stmt = connection->createStatement();
        resultat = stmt->executeQuery("SELECT * FROM transfert_tb WHERE code = " + std::to_string(codeTransf));
        if (resultat->next())
            trouve = 1;
    }
    catch (sql::SQLException& e) {
        std::cout << "# ERR: " << e.what();
        std::cout << " (code erreur MySQL: " << e.getErrorCode();
        std::cout << ", EtatSQL: " << e.getSQLState() << ")" << std::endl;
    }

    if (afficher && trouve == 1) {
        tabulate::Table affichage;
        StringTransfert valueFetched;

        valueFetched.nomExpe = resultat->getString(2);
        valueFetched.adExpe = resultat->getString(3);
        valueFetched.nomBene = resultat->getString(4);
        valueFetched.adBene = resultat->getString(5);
        valueFetched.montant = resultat->getString(10);
        valueFetched.dateExpe = resultat->getString(7);
        affichage.add_row({ "NOM EXPEDITEUR", "ADRESSE EXPEDITEUR", "NOM BENEFICIAIRE", "ADRESSE BENEFICIAIRE", "MONTANT", "DATE EXPEDITION" });
        affichage.add_row({ valueFetched.nomExpe, valueFetched.adExpe, valueFetched.nomBene, valueFetched.adBene, valueFetched.montant, valueFetched.dateExpe });

        affichage[0].format()
            .padding_bottom(1)
            .padding_top(21)
            .font_background_color(tabulate::Color::white)
            .font_color(tabulate::Color::green)
            .font_style({ tabulate::FontStyle::underline })
            .font_align(tabulate::FontAlign::center);

        affichage.format()
            .font_align(tabulate::FontAlign::center)
            .width(23);

        std::cout << affichage << std::endl;
    }

    delete stmt;
    delete resultat;
    delete connection;

    if (trouve == 1) return true;
    return false;
}

void remove_transfer() {
    long long code = 0;

    animationSansRetour("### Entrez le code du transfert >>> ");
    secureEntry(code);
    if (!find_transfer(code, true)) {
        affichageErreur("### CE CODE NE FIGURE PAS DANS LA BASE DE DONNEES ###", rouge);

        delete connection;
        delete stmt;
        delete resultat;
        return;
    }
    long long carteId = 0;

    animationSansRetour("### Entrez le num�ro de la carte ID du b�n�ficiaire >>> ");
    secureEntry(carteId);
    connect_db();
    stmt = connection->createStatement();
    resultat = stmt->executeQuery("SELECT * FROM transfert_tb WHERE code = '" + std::to_string(code) + "' AND carteIdBene = '" + std::to_string(carteId) + "'");
    if (resultat->next() ) {
        int confirmation = 0;

        animationSansRetour("### Confirmez le r�trait en tapant 1 ou autre chiffre pour annuler >>> ");
        secureEntry(confirmation);
        if (confirmation == 1) {
            if (resultat->getInt(11) != 1 && resultat->getInt(12) != 1) {
                try {
                    std::string date;
                    std::string jour = std::to_string(day);
                    std::string mois = std::to_string(month);
                    std::string annee = std::to_string(year);

                    if (day < 10) jour = "0" + jour;
                    if (month < 10) mois = "0" + mois;

                    date += jour;
                    date += "/";
                    date += mois;
                    date += "/";
                    date += annee;

                    pstmt = connection->prepareStatement("UPDATE transfert_tb SET retirer = '1', dateRetrait = ? WHERE code = ?");
                    pstmt->setString(1, date);
                    pstmt->setString(2, std::to_string(code));
                    affichageErreur("### RETRAIT EFFECTUEE AVEC SUCCES  ###", vert);
                }
                catch (sql::SQLException& e) {
                    std::cout << "# ERR: " << e.what();
                    std::cout << " (code erreur MySQL: " << e.getErrorCode();
                    std::cout << ", EtatSQL: " << e.getSQLState() << ")" << std::endl;
                }
            }
            else {
                affichageErreur("### CE TRANSFERT A DEJA ETE RETIRE OU ANNULE ###", rouge);
            }
        }
        else {
            affichageErreur("### RETRAIT ANNULE ###", rouge);
        }
    }
    else {
        affichageErreur("### ERREUR CE NUMERO DE CARTE ID NE FIGURE PAS DANS LA BASE DE DONNEES ###", rouge);
    }

    delete stmt;
    delete pstmt;
    delete resultat;
    delete connection;
}

void cancel_transfer() {
    long long code = 0;

    animationSansRetour("### Entrez le code du transfert >>> ");
    secureEntry(code);
    if (!find_transfer(code, true)) {
        affichageErreur("### CE CODE NE FIGURE PAS DANS LA BASE DE DONNEES ###", rouge);

        delete connection;
        delete stmt;
        delete resultat;
        return;
    }
    long long carteId = 0;

    animationSansRetour("### Entrez le num�ro de la carte ID du b�n�ficiaire >>> ");
    secureEntry(carteId);
    connect_db();
    stmt = connection->createStatement();
    resultat = stmt->executeQuery("SELECT * FROM transfert_tb WHERE code = '" + std::to_string(code) +"' AND carteIdBene = '" + std::to_string(carteId) + "'");
    if (resultat->next()) {
        int confirmation = 0;

        animationSansRetour("### Confirmez l'annulation en tapant 1 ou autre chiffre pour abandonner >>> ");
        secureEntry(confirmation);
        if (confirmation == 1) {
            if (resultat->getInt(11) != 1 && resultat->getInt(12) != 1) {
                try {
                    std::string date;
                    std::string jour = std::to_string(day);
                    std::string mois = std::to_string(month);
                    std::string annee = std::to_string(year);

                    if (day < 10) jour = "0" + jour;
                    if (month < 10) mois = "0" + mois;

                    date += jour;
                    date += "/";
                    date += mois;
                    date += "/";
                    date += annee;
                    pstmt = connection->prepareStatement("UPDATE transfert_tb SET annuler = '1', dateAnnul = ? WHERE code = ?");
                    pstmt->setString(1, date);
                    pstmt->setString(2, std::to_string(code));
                    pstmt->execute();
                    affichageErreur("### LE TRANSFERT A ETE ANNULE AVEC SUCCES  ###", vert);
                }
                catch (sql::SQLException& e) {
                    std::cout << "# ERR: " << e.what();
                    std::cout << " (code erreur MySQL: " << e.getErrorCode();
                    std::cout << ", EtatSQL: " << e.getSQLState() << ")" << std::endl;
                }
            }
            else {
                affichageErreur("### CE TRANSFERT A DEJA ETE ANNULE OU RETIRE ###", rouge);
            }
        }
        else {
            affichageErreur("### ANNULATION DU TRANSFERT ABANDONNEE ###", rouge);
        }
    }
    else {
        affichageErreur("### ERREUR CE NUMERO DE CARTE ID NE FIGURE PAS DANS LA BASE DE DONNEES ###", rouge);
    }

    delete stmt;
    delete pstmt;
    delete resultat;
    delete connection;
}

void delete_transfer() {
    long long code = 0;

    animationSansRetour("### Entrez le code du transfert >>> ");
    secureEntry(code);
    if (find_transfer(code, true)) {
        int confirmation = 0;

        animationSansRetour("### Confirmez la suppression en tapant 1 ou autre chiffre pour annuler >>> ");
        secureEntry(confirmation);
        if (confirmation == 1) {
            try {
                connect_db();
                pstmt = connection->prepareStatement("DELETE FROM transfert_tb WHERE code = ?");
                pstmt->setString(1, std::to_string(code));
                pstmt->execute();
                affichageErreur("### SUPPRESSION EFFECTUEE AVEC SUCCES  ###", vert);
            }
            catch (sql::SQLException& e) {
                std::cout << "# ERR: " << e.what();
                std::cout << " (code erreur MySQL: " << e.getErrorCode();
                std::cout << ", EtatSQL: " << e.getSQLState() << ")" << std::endl;
            }
        }
        else {
            affichageErreur("### SUPPRESSION ANNULE ###", rouge);
        }
    }
    else {
        affichageErreur("### CE CODE NE FIGURE PAS DANS LA BASE DE DONNEES ###", rouge);
    }

    delete pstmt;
    delete connection;
}

void alter_transfer() {
    try {
        long long code = 0;

        animationSansRetour("### Entrez le code du transfert >>> ");
        secureEntry(code);
        if (find_transfer(code, true)) {
            tabulate::Table menuChoice;
            menuChoice.add_row({ "CHOIX" });
            menuChoice.add_row({ "[1]  MODIFIER LE NOM DE L'EXPEDITEUR " });
            menuChoice.add_row({ "[2]  MODIFIER L'ADRESSE DE L'EXPEDITEUR " });
            menuChoice.add_row({ "[3]  MODIFIER LE NOM DU BENEFICIAIRE " });
            menuChoice.add_row({ "[4]  MODIFIER L'ADRESSE DU BENEFICIAIRE " });
            menuChoice.add_row({ "[5]  MODIFIER LE NUMERO DE CARTE ID DU BENEFICIAIRE " });

            menuChoice.format()
                .width(143)
                .font_color(tabulate::Color::green);

            menuChoice[0].format()
                .padding_top(1)
                .padding_bottom(1)
                .font_style({ tabulate::FontStyle::underline })
                .font_background_color(tabulate::Color::white)
                .font_align(tabulate::FontAlign::center)
                .font_color(tabulate::Color::green);

            std::cout << menuChoice << std::endl;
            int choix = 0;
            StringTransfert setValue;
            choice:
            animationSansRetour("### Entrez votre choix >>> ");
            secureEntry(choix);
            switch (choix) {
                case 1:
                    animationSansRetour("### Entrez le nom de l'exp�diteur >>> ");
                    secureEntry(setValue.nomExpe);
                    connect_db();
                    pstmt = connection->prepareStatement("UPDATE transfert_tb SET nomExpe = ? WHERE code = '" + std::to_string(code) + "'");
                    pstmt->setString(1, setValue.nomExpe);
                    pstmt->execute();
                    break;
                case 2:
                    animationSansRetour("### Entrez l'adresse de l'exp�diteur (Ville Quartier) >>> ");
                    secureEntry(setValue.adExpe);
                    connect_db();
                    pstmt = connection->prepareStatement("UPDATE transfert_tb SET adresseExpe = ? WHERE code = '" + std::to_string(code) + "'");
                    pstmt->setString(1, setValue.adExpe);
                    pstmt->execute();
                    break;
                case 3:
                    animationSansRetour("### Entrez le nom du b�n�ficiaire >>> ");
                    secureEntry(setValue.nomBene);
                    connect_db();
                    pstmt = connection->prepareStatement("UPDATE transfert_tb SET nomBene = ? WHERE code = '" + std::to_string(code) + "'");
                    pstmt->setString(1, setValue.nomBene);
                    pstmt->execute();
                    break;
                case 4:
                    animationSansRetour("### Entrez l'adresse du b�n�ficiaire (Ville Quartier) >>> ");
                    secureEntry(setValue.adBene);
                    connect_db();
                    pstmt = connection->prepareStatement("UPDATE transfert_tb SET adresseBene = ? WHERE code = '" + std::to_string(code) + "'");
                    pstmt->setString(1, setValue.adBene);
                    pstmt->execute();
                    break;
                case 5:
                    animationSansRetour("### Entrez le num�ro de carte ID du b�n�ficiaire >>> ");
                    secureEntry(setValue.carteIdBene);
                    connect_db();
                    pstmt = connection->prepareStatement("UPDATE transfert_tb SET carteIdBene = ? WHERE code = '" + std::to_string(code) + "'");
                    pstmt->setString(1, setValue.carteIdBene);
                    pstmt->execute();
                    break;
                default:
                    affichageErreur("### ERREUR, ENTREZ UN CHOIX DISPONIBLE ###", rouge);
                    goto choice;
            }

            affichageErreur("### MODIFICATION EFFECTUEE AVEC SUCCES ###", vert);

            delete pstmt;
            delete connection;
        }
        else {
            affichageErreur("### CE CODE NE FIGURE PAS DANS LA BASE DE DONNEES ###", rouge);
        }
    }
    catch (sql::SQLException& e) {
        std::cout << "# ERR: " << e.what();
        std::cout << " (code erreur MySQL: " << e.getErrorCode();
        std::cout << ", EtatSQL: " << e.getSQLState() << ")" << std::endl;
    }
}

void transfer_situation() {
    try {
        connect_db();
        stmt = connection->createStatement();
        resultat = stmt->executeQuery("SELECT COUNT(code), SUM(montant) FROM transfert_tb WHERE annuler <> 1");
        if (resultat->next()) {
            tabulate::Table affichage;
            int nombreDepots = resultat->getInt(1);
            long long totalAmount = resultat->getInt64(2);

            affichage.add_row({ "NOMBRE DE TRANSFERTS", "MONTANT TOTAL DES TRANSFERTS" });
            std::string montant = std::to_string(totalAmount) + ".00 GNF";
            std::string nbr_depots = std::to_string(nombreDepots);
            affichage.add_row({ nbr_depots, montant });
            
            affichage[0].format()
                .padding_bottom(1)
                .padding_top(1)
                .font_background_color(tabulate::Color::white)
                .font_color(tabulate::Color::green)
                .font_style({ tabulate::FontStyle::underline })
                .font_align(tabulate::FontAlign::center);

            affichage.format()
                .font_align(tabulate::FontAlign::center)
                .width(71);

            std::cout << affichage << std::endl;
        }
        else {
            affichageErreur("### AUCUN TRANSFERT EFFECTUE POUR LE MOMENT ###", rouge);
        }
    }
    catch (sql::SQLException& e) {
        std::cout << "# ERR: " << e.what();
        std::cout << " (code erreur MySQL: " << e.getErrorCode();
        std::cout << ", EtatSQL: " << e.getSQLState() << ")" << std::endl;
    }

    delete stmt;
    delete resultat;
    delete connection;
}

void list_all_transfer(){
    try {
        connect_db();
        stmt = connection->createStatement();
        resultat = stmt->executeQuery("SELECT * FROM transfert_tb ORDER BY nomExpe ASC");
        if (resultat != nullptr) {
            tabulate::Table affichage;
            StringTransfert valueFetched;

            affichage.add_row({ "NOM EXPEDITEUR", "ADRESSE EXPEDITEUR", "NOM BENEFICIAIRE", "ADRESSE BENEFICIAIRE", "MONTANT", "DATE EXPEDITION" });
            while (resultat->next()) {
                valueFetched.nomExpe = resultat->getString(2);
                valueFetched.adExpe = resultat->getString(3);
                valueFetched.nomBene = resultat->getString(4);
                valueFetched.adBene = resultat->getString(5);
                valueFetched.montant = resultat->getString(10);
                valueFetched.dateExpe = resultat->getString(7);
                affichage.add_row({ valueFetched.nomExpe, valueFetched.adExpe, valueFetched.nomBene, valueFetched.adBene, valueFetched.montant, valueFetched.dateExpe });
            }

            affichage[0].format()
                .padding_bottom(1)
                .padding_top(1)
                .font_background_color(tabulate::Color::white)
                .font_color(tabulate::Color::green)
                .font_style({ tabulate::FontStyle::underline })
                .font_align(tabulate::FontAlign::center);

            affichage.format()
                .font_align(tabulate::FontAlign::center)
                .width(26);

            std::cout << affichage << std::endl;
        }
        else {
            affichageErreur("### AUCUN TRANSFERT EFFECTUE POUR LE MOMENT ###", rouge);
        }
    }
    catch (sql::SQLException& e) {
        std::cout << "# ERR: " << e.what();
        std::cout << " (code erreur MySQL: " << e.getErrorCode();
        std::cout << ", EtatSQL: " << e.getSQLState() << ")" << std::endl;
    }

    delete stmt;
    delete resultat;
    delete connection;
}

void list_all_transfer_onDate() {
    try {
        std::string date;
        animationSansRetour("### ENTREZ LA DATE (JJ/MM/AAAA) >>> ");
        secureEntry(date);
        connect_db();
        stmt = connection->createStatement();
        resultat = stmt->executeQuery("SELECT * FROM transfert_tb WHERE dateExpe = '" + date + "' ORDER BY nomExpe ASC");
        if (resultat != nullptr) {
            tabulate::Table affichage;
            StringTransfert valueFetched;

            affichage.add_row({ "NOM EXPEDITEUR", "ADRESSE EXPEDITEUR", "NOM BENEFICIAIRE", "ADRESSE BENEFICIAIRE", "MONTANT", "DATE EXPEDITION" });
            while (resultat->next()) {
                valueFetched.nomExpe = resultat->getString(2);
                valueFetched.adExpe = resultat->getString(3);
                valueFetched.nomBene = resultat->getString(4);
                valueFetched.adBene = resultat->getString(5);
                valueFetched.montant = resultat->getString(10);
                valueFetched.dateExpe = resultat->getString(7);
                affichage.add_row({ valueFetched.nomExpe, valueFetched.adExpe, valueFetched.nomBene, valueFetched.adBene, valueFetched.montant, valueFetched.dateExpe });
            }

            affichage[0].format()
                .padding_bottom(1)
                .padding_top(1)
                .font_background_color(tabulate::Color::white)
                .font_color(tabulate::Color::green)
                .font_style({ tabulate::FontStyle::underline })
                .font_align(tabulate::FontAlign::center);

            affichage.format()
                .font_align(tabulate::FontAlign::center)
                .width(26);

            std::cout << affichage << std::endl;
        }
        else {
            affichageErreur("### AUCUN TRANSFERT EFFECTUE POUR LE MOMENT ###", rouge);
        }
    }
    catch (sql::SQLException& e) {
        std::cout << "# ERR: " << e.what();
        std::cout << " (code erreur MySQL: " << e.getErrorCode();
        std::cout << ", EtatSQL: " << e.getSQLState() << ")" << std::endl;
    }

    delete stmt;
    delete resultat;
    delete connection;
}

void list_all_transfer_betweenTwoDates() {
    try {
        std::string date1;
        std::string date2;
        animationSansRetour("### ENTREZ LA 1�re DATE (JJ/MM/AAAA) >>> ");
        secureEntry(date1);
        animationSansRetour("### ENTREZ LA 2e DATE (JJ/MM/AAAA) >>> ");
        secureEntry(date2);
        connect_db();
        stmt = connection->createStatement();
        resultat = stmt->executeQuery("SELECT * FROM transfert_tb WHERE dateExpe BETWEEN '" + date1 + "' AND '" + date2 + "' ORDER BY nomExpe ASC");
        if (resultat != nullptr) {
            tabulate::Table affichage;
            StringTransfert valueFetched;

            affichage.add_row({ "NOM EXPEDITEUR", "ADRESSE EXPEDITEUR", "NOM BENEFICIAIRE", "ADRESSE BENEFICIAIRE", "MONTANT", "DATE EXPEDITION" });
            while (resultat->next()) {
                valueFetched.nomExpe = resultat->getString(2);
                valueFetched.adExpe = resultat->getString(3);
                valueFetched.nomBene = resultat->getString(4);
                valueFetched.adBene = resultat->getString(5);
                valueFetched.montant = resultat->getString(10);
                valueFetched.dateExpe = resultat->getString(7);
                affichage.add_row({ valueFetched.nomExpe, valueFetched.adExpe, valueFetched.nomBene, valueFetched.adBene, valueFetched.montant, valueFetched.dateExpe });
            }

            affichage[0].format()
                .padding_bottom(1)
                .padding_top(1)
                .font_background_color(tabulate::Color::white)
                .font_color(tabulate::Color::green)
                .font_style({ tabulate::FontStyle::underline })
                .font_align(tabulate::FontAlign::center);

            affichage.format()
                .font_align(tabulate::FontAlign::center)
                .width(26);

            std::cout << affichage << std::endl;
        }
        else {
            affichageErreur("### AUCUN TRANSFERT EFFECTUE POUR LE MOMENT ###", rouge);
        }
    }
    catch (sql::SQLException& e) {
        std::cout << "# ERR: " << e.what();
        std::cout << " (code erreur MySQL: " << e.getErrorCode();
        std::cout << ", EtatSQL: " << e.getSQLState() << ")" << std::endl;
    }

    delete stmt;
    delete resultat;
    delete connection;
}

void list_all_transfer_removedOnDate() {
    try {
        std::string date;
        animationSansRetour("### ENTREZ LA DATE (JJ/MM/AAAA) >>> ");
        secureEntry(date);
        connect_db();
        stmt = connection->createStatement();
        resultat = stmt->executeQuery("SELECT * FROM transfert_tb WHERE retirer = '1' AND dateRetrait = '" + date + "' ORDER BY nomExpe ASC");
        if (resultat != nullptr) {
            tabulate::Table affichage;
            StringTransfert valueFetched;

            affichage.add_row({ "NOM EXPEDITEUR", "ADRESSE EXPEDITEUR", "NOM BENEFICIAIRE", "ADRESSE BENEFICIAIRE", "MONTANT", "DATE EXPEDITION" });
            while (resultat->next()) {
                valueFetched.nomExpe = resultat->getString(2);
                valueFetched.adExpe = resultat->getString(3);
                valueFetched.nomBene = resultat->getString(4);
                valueFetched.adBene = resultat->getString(5);
                valueFetched.montant = resultat->getString(10);
                valueFetched.dateExpe = resultat->getString(7);
                affichage.add_row({ valueFetched.nomExpe, valueFetched.adExpe, valueFetched.nomBene, valueFetched.adBene, valueFetched.montant, valueFetched.dateExpe });
            }

            affichage[0].format()
                .padding_bottom(1)
                .padding_top(1)
                .font_background_color(tabulate::Color::white)
                .font_color(tabulate::Color::green)
                .font_style({ tabulate::FontStyle::underline })
                .font_align(tabulate::FontAlign::center);

            affichage.format()
                .font_align(tabulate::FontAlign::center)
                .width(26);

            std::cout << affichage << std::endl;
        }
        else {
            affichageErreur("### AUCUN TRANSFERT EFFECTUE POUR LE MOMENT ###", rouge);
        }
    }
    catch (sql::SQLException& e) {
        std::cout << "# ERR: " << e.what();
        std::cout << " (code erreur MySQL: " << e.getErrorCode();
        std::cout << ", EtatSQL: " << e.getSQLState() << ")" << std::endl;
    }

    delete stmt;
    delete resultat;
    delete connection;
}

void list_all_transfer_removed_betweenTwoDates() {
    try {
        std::string date1;
        std::string date2;
        animationSansRetour("### ENTREZ LA 1�re DATE (JJ/MM/AAAA) >>> ");
        secureEntry(date1);
        animationSansRetour("### ENTREZ LA 2e DATE (JJ/MM/AAAA) >>> ");
        secureEntry(date2);
        connect_db();
        stmt = connection->createStatement();
        resultat = stmt->executeQuery("SELECT * FROM transfert_tb WHERE retirer = '1' AND dateRetrait BETWEEN '" + date1 + "' AND '" + date2 + "' ORDER BY nomExpe ASC");
        if (resultat != nullptr) {
            tabulate::Table affichage;
            StringTransfert valueFetched;

            affichage.add_row({ "NOM EXPEDITEUR", "ADRESSE EXPEDITEUR", "NOM BENEFICIAIRE", "ADRESSE BENEFICIAIRE", "MONTANT", "DATE EXPEDITION" });
            while (resultat->next()) {
                valueFetched.nomExpe = resultat->getString(2);
                valueFetched.adExpe = resultat->getString(3);
                valueFetched.nomBene = resultat->getString(4);
                valueFetched.adBene = resultat->getString(5);
                valueFetched.montant = resultat->getString(10);
                valueFetched.dateExpe = resultat->getString(7);
                affichage.add_row({ valueFetched.nomExpe, valueFetched.adExpe, valueFetched.nomBene, valueFetched.adBene, valueFetched.montant, valueFetched.dateExpe });
            }

            affichage[0].format()
                .padding_bottom(1)
                .padding_top(1)
                .font_background_color(tabulate::Color::white)
                .font_color(tabulate::Color::green)
                .font_style({ tabulate::FontStyle::underline })
                .font_align(tabulate::FontAlign::center);

            affichage.format()
                .font_align(tabulate::FontAlign::center)
                .width(26);

            std::cout << affichage << std::endl;
        }
        else {
            affichageErreur("### AUCUN TRANSFERT EFFECTUE POUR LE MOMENT ###", rouge);
        }
    }
    catch (sql::SQLException& e) {
        std::cout << "# ERR: " << e.what();
        std::cout << " (code erreur MySQL: " << e.getErrorCode();
        std::cout << ", EtatSQL: " << e.getSQLState() << ")" << std::endl;
    }

    delete stmt;
    delete resultat;
    delete connection;
}

void list_all_transfer_removed() {
    try {
        connect_db();
        stmt = connection->createStatement();
        resultat = stmt->executeQuery("SELECT * FROM transfert_tb WHERE retirer = '1' ORDER BY nomExpe ASC");
        if (resultat != nullptr) {
            tabulate::Table affichage;
            StringTransfert valueFetched;

            affichage.add_row({ "NOM EXPEDITEUR", "ADRESSE EXPEDITEUR", "NOM BENEFICIAIRE", "ADRESSE BENEFICIAIRE", "MONTANT", "DATE EXPEDITION", "DATE DE RETRAIT"});
            while (resultat->next()) {
                valueFetched.nomExpe = resultat->getString(2);
                valueFetched.adExpe = resultat->getString(3);
                valueFetched.nomBene = resultat->getString(4);
                valueFetched.adBene = resultat->getString(5);
                valueFetched.montant = resultat->getString(10);
                valueFetched.dateExpe = resultat->getString(7);
                valueFetched.dateRetrait = resultat->getString(8);
                affichage.add_row({ valueFetched.nomExpe, valueFetched.adExpe, valueFetched.nomBene, valueFetched.adBene, valueFetched.montant, valueFetched.dateExpe, valueFetched.dateRetrait });
            }

            affichage[0].format()
                .padding_bottom(1)
                .padding_top(1)
                .font_background_color(tabulate::Color::white)
                .font_color(tabulate::Color::green)
                .font_style({ tabulate::FontStyle::underline })
                .font_align(tabulate::FontAlign::center);

            affichage.format()
                .font_align(tabulate::FontAlign::center)
                .width(29);

            std::cout << affichage << std::endl;
        }
        else {
            affichageErreur("### AUCUN TRANSFERT N'A ETE RETIRE POUR LE MOMENT ###", rouge);
        }
    }
    catch (sql::SQLException& e) {
        std::cout << "# ERR: " << e.what();
        std::cout << " (code erreur MySQL: " << e.getErrorCode();
        std::cout << ", EtatSQL: " << e.getSQLState() << ")" << std::endl;
    }

    delete stmt;
    delete resultat;
    delete connection;
}

void list_all_transfer_cancelled() {
    try {
        connect_db();
        stmt = connection->createStatement();
        resultat = stmt->executeQuery("SELECT * FROM transfert_tb WHERE annuler = '1' ORDER BY nomExpe ASC");
        if (resultat != nullptr) {
            tabulate::Table affichage;
            StringTransfert valueFetched;

            affichage.add_row({ "NOM EXPEDITEUR", "ADRESSE EXPEDITEUR", "NOM BENEFICIAIRE", "ADRESSE BENEFICIAIRE", "MONTANT", "DATE EXPEDITION", "DATE ANNULATION"});
            while (resultat->next()) {
                valueFetched.nomExpe = resultat->getString(2);
                valueFetched.adExpe = resultat->getString(3);
                valueFetched.nomBene = resultat->getString(4);
                valueFetched.adBene = resultat->getString(5);
                valueFetched.montant = resultat->getString(10);
                valueFetched.dateExpe = resultat->getString(7);
                valueFetched.dateAnnu = resultat->getString(9);
                affichage.add_row({ valueFetched.nomExpe, valueFetched.adExpe, valueFetched.nomBene, valueFetched.adBene, valueFetched.montant, valueFetched.dateExpe, valueFetched.dateAnnu });
            }

            affichage[0].format()
                .padding_bottom(1)
                .padding_top(1)
                .font_background_color(tabulate::Color::white)
                .font_color(tabulate::Color::green)
                .font_style({ tabulate::FontStyle::underline })
                .font_align(tabulate::FontAlign::center);

            affichage.format()
                .font_align(tabulate::FontAlign::center)
                .width(29);

            std::cout << affichage << std::endl;
        }
        else {
            affichageErreur("### AUCUN TRANSFERT N'A ETE ANNULE POUR LE MOMENT ###", rouge);
        }
    }
    catch (sql::SQLException& e) {
        std::cout << "# ERR: " << e.what();
        std::cout << " (code erreur MySQL: " << e.getErrorCode();
        std::cout << ", EtatSQL: " << e.getSQLState() << ")" << std::endl;
    }

    delete stmt;
    delete resultat;
    delete connection;
}

void secureEntry(std::string& variable) {
    std::cin >> variable;
    while (std::cin.fail()) {
        changerCouleur(rouge);
        animationSansRetour("\n ### Entree invalide. Recommencez >>> ");
        changerCouleur(blanc);
        std::cin.clear();
        std::cin.ignore();
        std::cin >> variable;
    }
}

void secureEntry(int& variable) {
    std::cin >> variable;
    while (std::cin.fail()) {
        changerCouleur(rouge);
        animationSansRetour("\n ### Entree invalide. Recommencez >>> ");
        changerCouleur(blanc);
        std::cin.clear();
        std::cin.ignore();
        std::cin >> variable;
    }
}

void secureEntry(long& variable) {
    std::cin >> variable;
    while (std::cin.fail()) {
        changerCouleur(rouge);
        animationSansRetour("\n ### Entree invalide. Recommencez >>> ");
        changerCouleur(blanc);
        std::cin.clear();
        std::cin.ignore();
        std::cin >> variable;
    }
}

void secureEntry(double& variable) {
    std::cin >> variable;
    while (std::cin.fail()) {
        changerCouleur(rouge);
        animationSansRetour("\n ### Entree invalide. Recommencez >>> ");
        changerCouleur(blanc);
        std::cin.clear();
        std::cin.ignore();
        std::cin >> variable;
    }
}

void secureEntry(long long& variable) {
    std::cin >> variable;
    while (std::cin.fail()) {
        changerCouleur(rouge);
        animationSansRetour("\n ### Entree invalide. Recommencez >>> ");
        changerCouleur(blanc);
        std::cin.clear();
        std::cin.ignore();
        std::cin >> variable;
    }
}

void animation(const std::string& chaine) {
    SetConsoleOutputCP(1252);
    for (int i = 0; i < chaine.length(); i++) {
        std::cout << chaine[i];
        Sleep(20);
    }

    std::cout << std::endl;
}

void animationSansRetour(const std::string& chaine) {
    SetConsoleOutputCP(1252);
    for (int i = 0; i < chaine.length(); i++) {
        std::cout << chaine[i];
        Sleep(20);
    }
}

void affichageErreur(const std::string& message, color couleur) {
    changerCouleur(couleur);
    animation(message);
    changerCouleur(blanc);
}

void clear() {
    Sleep(2000);
    system("cls");
}

void changerCouleur(WORD numeroCouleur) {
    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);

    SetConsoleTextAttribute(hStdOut, numeroCouleur);
}

void exitProgram() {
    MessageBox(nullptr, TEXT("MERCI D'AVOIR UTILISE NOTRE PROGRAMME \n\n\n\t\t A BIENTOT"), TEXT("FlashTrans"), MB_OK);
}