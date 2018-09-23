int cargo;
	int num_rand;
	for (int i = 0; i < 5; i++)	{
		shop[i] = rand() % 200 + 10000;
	}
	
	while (1) {

		cargo = rand() % 200 + 10000;
		
		/*
        * Находит первый попавшийся незанятый магазин,
        * начиная от какого-то случайного.
        */
		num_rand = rand()%5;
		for (; num_rand < SHOP_NUM; num_rand++) {
            printf("\number shop for loader #%i", num_rand);
            if (!pthread_mutex_trylock(&mut[num_rand])) {
                break;
            }
        }
		
		/*
         * Если найти свободный магазин получилось - загружаем товар,
         * если нет, то нет
         */
        if (num_rand != SHOP_NUM) {
            printf("\nLoader delivers %i goods to shop %i\n", cargo, num_rand);

            shop[num_rand] += cargo;

            pthread_mutex_unlock(&mut[num_rand]);
        } else {
            printf("\nLoader hasn't deliver any goods\n");
        }
		
		for( int i = 0; i < 5; i++){
			printf ("%i \n", shop[i]);
		}
		//заргузчик спит 2 секунды
		sleep(2);
	}
	return 0;
}


//функция покупателей для утоления их потребностей
void* bshop(void* demand1){
	int *demand = demand1;
	int num_rand;
	long int tid = pthread_self();
	while (1) {
        /*
         * Клиент просыпается.
         */
        printf("\nBuyer#%li wakes up (need for death: %i)\n", tid, *demand);
        /*
         * Находит первый попавшийся незанятый магазин в котором есть товары,
         * начиная от какого-то случайного.
         */
        num_rand = rand()%5;
        for (; num_rand < SHOP_NUM; num_rand++) {
            printf("\nBuyer# %li goes to shop # %i", tid, num_rand);
            if (shop[num_rand] != 0 && !pthread_mutex_trylock(&mut[num_rand])) {
                break;
            }
        }
        /*
         * Если найти свободный магазин получилось - забираем весь товар,
         * если нет, то нет
         */
        if (num_rand != SHOP_NUM) {
            printf("\nBuyer#%li eats %i goods from shop#%i\n", tid,
				shop[num_rand], num_rand);

            demand -= shop[num_rand];
            shop[num_rand] = 0;

            pthread_mutex_unlock(&mut[num_rand]);
        } else {
            printf("\nBuyer#%li hasn't eat any goods\n", tid);
        }

        /*
         * Если покупатель наелся, то он умирает, иначе - засыпает.
         */
        if (demand <= 0) {
            printf("\nBuyer#%li blows up\n", tid);
            break;
        }
        printf("\nBuyer#%li falls asleep\n", tid);
        sleep(1);
    }
	
	return 0;
	
}


int main(){
	int demand[3];
	srand(time(NULL));
	pthread_t buyer[3], loader;
	pthread_create(&loader, NULL, lshop, NULL);
	
	for (int i = 0; i < 3; i++){
		demand[i] = rand() % 1000 + 9500;
		pthread_create(&(buyer[i]), NULL, bshop, &demand[i]);
	}
	for (int i = 0; i < 3; i++){
		pthread_join(buyer[i], NULL);
	}
	pthread_join(loader, NULL);
	
	
	
	return 0;
}
