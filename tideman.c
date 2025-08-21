#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];

// Each pair has a winner, loser
typedef struct
{
    int winner;
    int loser;
} pair;

// Array of candidates
char *candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;
int voter_count;
int candy = 0;

// Function prototypes
bool vote(int rank, char *name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i] = argv[i + 1];
    }

    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
            preferences[i][j] = 0;
        }
    }

    pair_count = 0;
    printf("Number of voters: ");
    scanf ("%d", &voter_count);

    // Query for votes
    char *name = malloc(15 * sizeof (char));
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            printf("Rank %d: ", j + 1);
            scanf("%s", name);

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }

        record_preferences(ranks);
        printf("\n");
    }
    free (name);
    add_pairs();
    int f = 0;
    for (int i = 0; i < candidate_count*(candidate_count - 1)/2 - 1; i++)
    {
        for (int j = 1+i; j < candidate_count*(candidate_count - 1)/2; j++)
        {
            int g = preferences[pairs[i].winner][pairs[i].loser] - preferences[pairs[i].loser][pairs[i].winner];
            int h = preferences[pairs[j].winner][pairs[j].loser] - preferences[pairs[j].loser][pairs[j].winner];
            if (h > g)
            {
              f++;
            }    
            if (g < h)
            {
                f++;
            }
        }
    }
    if (f == 0)
    {
        printf ("Tie!\n");
        return 0;
    }
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}

// Update ranks given a new vote
bool vote(int rank, char *name, int ranks[])
{
    for (int i = 0; i < candidate_count; i++)
    {
        if (strcmp (name, candidates[i]) == 0)
        {
            ranks[rank] = i;
            return true;
        }
    }
    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 1 + i; j < candidate_count; j++)
        {
            preferences[ranks[i]][ranks[j]]++;
        }
    }
    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    int x = 0;
    for (int i= 0; i < candidate_count - 1; i++)
    {
        for (int j = i+1; j < candidate_count; j++)
        {
            if(preferences[i][j] > preferences[j][i])
            {
                pairs[x].winner = i;
                pairs[x].loser = j;
                x++;
            }    
            else if (preferences[i][j] < preferences[j][i])
            {
                pairs[x].winner = j;
                pairs[x].loser = i;
                x++;
            }
        }
    }  
}
// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    pair tmp;
    for (int i = 0; i < candidate_count*(candidate_count - 1)/2 - 1; i++)
    {
        for (int j = 1+i; j < candidate_count*(candidate_count - 1)/2; j++)
        {
            int a = preferences[pairs[i].winner][pairs[i].loser] - preferences[pairs[i].loser][pairs[i].winner];
            int b = preferences[pairs[j].winner][pairs[j].loser] - preferences[pairs[j].loser][pairs[j].winner];
            if (b > a)
            {
                tmp.winner = pairs[i].winner;
                tmp.loser = pairs[i].loser;
                pairs[i].winner = pairs[j].winner;
                pairs[i].loser = pairs[j].loser;
                pairs[j].winner = tmp.winner;
                pairs[j].loser = tmp.loser;
            }    
        }
    }
   return;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    int x[candidate_count*(candidate_count - 1)/2];
    for (int i = 0; i < candidate_count*(candidate_count - 1)/2; i++)
    {
        locked[pairs[i].winner][pairs[i].loser] = true;
        x[i] = pairs[i].loser;
        for (int j = 0; j < candidate_count; j++)
        {
            int y = 0;
            for (int k = 0; k < i + 1; k++)
            {
                if (j == x[k])
                {
                    y++;
                    break;
                }
            }
            if (y == 0)
            {
                goto jump_here;
            }
        }
        locked[pairs[i].winner][pairs[i].loser] = false;
        break;
        jump_here:
        ;
    }
    return;
}

// Print the winner of the election
void print_winner(void)
{
    for (int i = 0; i < candidate_count*(candidate_count - 1)/2; i++)
    {
        for (int j = 0; j < candidate_count*(candidate_count - 1)/2; j++)
        {
            if (locked[pairs[i].winner][pairs[j].loser] && pairs[j].loser == candy)
            {
                candy++;
                print_winner();
                if (candy == candidate_count)
                {
                    return;
                }
            }
        }
    }
    printf ("%s is the winner!\n", candidates[candy]);
    candy = candidate_count;
    return;
} 
