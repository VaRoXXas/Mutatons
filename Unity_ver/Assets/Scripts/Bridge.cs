using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Bridge : MonoBehaviour
{
    private bool isOpen, currentlyRotating;
    [SerializeField] private GameObject leftPartPivot, rightPartPivot;
    private static float timeToSwitchState = 1.0f; // [in seconds] this is the time it takes to open or close the bridge

    void Awake()
    {
        isOpen = true; // because prefab IS OPEN by default
    }

    void Update()
    {
        if(currentlyRotating)
        {
            Vector3 addedRotation;
            if(isOpen)
            {
                addedRotation = new Vector3(0.0f, 0.0f , -90.0f / timeToSwitchState * Time.deltaTime);//addedRotation = new Vector3(0.0f, 0.0f , 90.0f / timeToSwitchState * Time.deltaTime);
                leftPartPivot.transform.Rotate(addedRotation);
                rightPartPivot.transform.Rotate(addedRotation);
                Debug.Log(leftPartPivot.transform.rotation.eulerAngles.z);
                if(leftPartPivot.transform.rotation.eulerAngles.z >= 90.0f) // -90 is represented as 270 in eulerAngles, "leftPartPivot.transform.rotation.eulerAngles.z >= 0.0f"
                {
                    if(leftPartPivot.transform.rotation.eulerAngles.y == 90.0f)
                    {
                        leftPartPivot.transform.rotation = Quaternion.Euler(0.0f, -270.0f, 0.0f);
                        rightPartPivot.transform.rotation = Quaternion.Euler(180.0f, -270.0f, 180.0f);
                        currentlyRotating = false;
                    }
                    else
                    {
                        leftPartPivot.transform.rotation = Quaternion.Euler(0.0f, 0.0f, 0.0f);
                        rightPartPivot.transform.rotation = Quaternion.Euler(0.0f, -180.0f, 0.0f);

                        currentlyRotating = false;
                    }
                    
                }
            }
            else
            {
                addedRotation = new Vector3(0.0f, 0.0f, 90.0f / timeToSwitchState * Time.deltaTime); //addedRotation = new Vector3(0.0f, 0.0f, -90.0f / timeToSwitchState * Time.deltaTime);
                leftPartPivot.transform.Rotate(addedRotation);
                rightPartPivot.transform.Rotate(addedRotation);
                if(leftPartPivot.transform.rotation.eulerAngles.z >= 90.0f) // -90 is represented as 270 in eulerAngles , "leftPartPivot.transform.rotation.eulerAngles.z <= 360.0f - 90.0f"
                {
                    if(leftPartPivot.transform.rotation.eulerAngles.y == 90.0f)
                    {
                        //leftPartPivot.transform.rotation = Quaternion.Euler(0.0f, -270.0f, -90.0f);
                        //rightPartPivot.transform.rotation = Quaternion.Euler(180.0f, -270.0f, 90.0f);
                        leftPartPivot.transform.rotation = Quaternion.Euler(0.0f, -270.0f, 90.0f);
                        rightPartPivot.transform.rotation = Quaternion.Euler(180.0f, -270.0f, -90.0f);
                        currentlyRotating = false;
                    }
                    else
                    {
                        //leftPartPivot.transform.rotation = Quaternion.Euler(0.0f, 0.0f, -90.0f);
                        //rightPartPivot.transform.rotation = Quaternion.Euler(0.0f, -180.0f, -90.0f);
                        leftPartPivot.transform.rotation = Quaternion.Euler(0.0f, 0.0f, 90.0f);
                        rightPartPivot.transform.rotation = Quaternion.Euler(0.0f, -180.0f, 90.0f);
                        currentlyRotating = false;
                    }
                    
                }
            }
        }
    }

    public void SetOpen(bool flag)
    {
        if(isOpen != flag)
        {
            isOpen = flag;
            currentlyRotating = true;
        }
    }
}
