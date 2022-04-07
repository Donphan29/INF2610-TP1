/*
 * TP4 - GestionMemoire.c
 * 
 * École Polytechnique de Montréal, Hiver 2022
 * Maxence Lefebvre, 2085581
 * Huy-Don Phan, 2074318
 * Section de laboratoire : 02 (B2)
 * 
 */

#include "./libs/lib.h"

/*
 * Calcul le numéro de page
 *
 * @param adresse virtuelle
 * @return numéro de page
 */
unsigned int calculerNumeroDePage(unsigned long adresse) {
    return adresse / TAILLE_PAGE_CADRE;
}

/*
 * Calcul le déplacement dans la page
 *
 * @param adresse virtuelle
 * @return déplacement dans la page
 */
unsigned long calculerDeplacementDansLaPage(unsigned long adresse) {
    return adresse % TAILLE_PAGE_CADRE;
}

/*
 * Calcul l'adresse complète
 *
 * @param numéro de page et déplacement dans la page
 * @return adresse complète
 */
unsigned long calculerAdresseComplete(unsigned int numeroDePage, unsigned long deplacementDansLaPage) {
    return numeroDePage * TAILLE_PAGE_CADRE + deplacementDansLaPage;
}

/*
 * Recherche une page dans le TLB
 * 
 * Si la page se trouve dans le TLB, la requête est assignée une adresse physique
 * correspondante avec la fonction calculerAdresseComplete. Sinon, celle-ci prend
 * la valeur 0. Les params sont mis à jour selon les cas.
 *
 * @param requête et mémoire
 */
void rechercherTLB(struct RequeteMemoire* req, struct SystemeMemoire* mem) {
	int numeroDePageReq = calculerNumeroDePage(req->adresseVirtuelle);

	for (int i = 0; i < TAILLE_TLB; i++) {
		if (mem->tlb->numeroPage[i] == numeroDePageReq && mem->tlb->entreeValide[i]) {
			req->adressePhysique = calculerAdresseComplete(mem->tlb->numeroCadre[i], calculerDeplacementDansLaPage(req->adresseVirtuelle));
			req->estDansTLB = 1;
			mem->tlb->dernierAcces[i] = req->date;
			return;
		}
	}
	req->adressePhysique = 0;
	req->estDansTLB = 0;
}

/*
 * Recherche une page dans la table des pages
 * 
 * Si la page se trouve dans la table des pages, la requête est assignée une 
 * adresse physique correspondante avec la fonction calculerAdresseComplete. 
 * Sinon, celle-ci prend la valeur 0. La requête est mis à jour selon le cas.
 *
 * @param requête et mémoire
 */
void rechercherTableDesPages(struct RequeteMemoire* req, struct SystemeMemoire* mem) {
	int numeroDePage = calculerNumeroDePage(req->adresseVirtuelle);

	if (mem->tp->entreeValide[numeroDePage]) {
		req->adressePhysique = calculerAdresseComplete(mem->tp->numeroCadre[numeroDePage], calculerDeplacementDansLaPage(req->adresseVirtuelle));
		req->estDansTablePages = 1;
	} else {
		req->adressePhysique = 0;
		req->estDansTablePages = 0;
	}
}

/*
 * Ajoute une page dans la mémoire principale
 * 
 * Dans le premier cadre disponible, la page précisé par la requête est ajoutée
 * à la mémoire principale en supposant qu'elle n'est jamais pleine. L'adresse
 * physique correspondante est calculée et stockée dans la requête et les paramètres
 * de la mémoire sont mis à jour.
 * 
 * @param requête et mémoire
 */
void ajouterDansMemoire(struct RequeteMemoire* req, struct SystemeMemoire* mem) {
	int numeroDePage = calculerNumeroDePage(req->adresseVirtuelle);

	for (int i = 0; i < TAILLE_MEMOIRE; i++) {
		if (mem->memoire->utilisee[i] == 0) {
			req->adressePhysique = calculerAdresseComplete(i, calculerDeplacementDansLaPage(req->adresseVirtuelle));
			mem->memoire->numeroPage[i] = numeroDePage;
			mem->memoire->dateCreation[i] = req->date;
			mem->memoire->dernierAcces[i] = req->date;
			mem->memoire->utilisee[i] = 1;
			break;
		}
	}
}

/*
 * Met à jour une page dans le TLB
 * 
 * Si le TLB est plein, une page est remplacée par celle précisée dans la requête
 * selon la politique FIFO. Sinon, celle-ci sera ajoutée dans le premier cadre
 * libre. Les autres paramres de la mémoire sont mis à jour.
 * 
 * @param requête et mémoire
 */
void mettreAJourTLB(struct RequeteMemoire* req, struct SystemeMemoire* mem) {
	int index = 0;
	int plein = 0;
	int premierElement = mem->tlb->dateCreation[index];

	for (int i =0; i < TAILLE_TLB; i++) {
		if (!mem->tlb->entreeValide[i]) {
			index = i;  
			break;
		}

		if(i == TAILLE_TLB - 1 & !plein) plein = 1; 
	}

	if (plein) {
		for (int i = 0; i < TAILLE_TLB; i++) {
			if (mem->tlb->dateCreation[i] < premierElement & plein) {
				index = i;
				premierElement = mem->tlb->dateCreation[i];
			}
		}
	}

	mem->tlb->numeroPage[index] = calculerNumeroDePage(req->adresseVirtuelle);
	mem->tlb->numeroCadre[index] = (req->adressePhysique - calculerDeplacementDansLaPage(req->adresseVirtuelle)) / TAILLE_PAGE_CADRE;
	mem->tlb->dateCreation[index] = req->date;
	mem->tlb->dernierAcces[index] = req->date;
	mem->tlb->entreeValide[index] = 1;
}

/* Fonction main */
int main() {
    evaluate(
		&calculerNumeroDePage, 
		&calculerDeplacementDansLaPage, 
		&calculerAdresseComplete, 
        &rechercherTLB, 
		&rechercherTableDesPages,
		&mettreAJourTLB,
		&ajouterDansMemoire
    );
    return 0;
}
