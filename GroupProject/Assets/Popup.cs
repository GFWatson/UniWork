using UnityEngine;
using System.Collections;

public class Popup : MonoBehaviour {
	
	public Texture dataBlock;
	
	public float x;
	public float y;
	
	void Start()
	{
		x = Random.Range (0, 400);
		y = Random.Range (0, 200);
	}
	
	public void OnGUI()
	{
		GUI.DrawTexture(new Rect(x, y, 100, 100), dataBlock, ScaleMode.StretchToFill, true, 0.0f);
	}
}