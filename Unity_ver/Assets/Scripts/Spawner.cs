using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Spawner : MonoBehaviour
{
    public GameObject Mutaton;
    int counter;
    public int number; 
    // Start is called before the first frame update
    void Start()
    {
        counter = 0;
        InvokeRepeating("Spawn", 1.5f, 1.5f);
    }

    void Spawn()
    {
        Instantiate(Mutaton, gameObject.transform.position, gameObject.transform.rotation);
        counter = counter + 1;
    }

    private void Update() 
    {
        if(counter==number)
        {
            CancelInvoke();
        }
    }
}
